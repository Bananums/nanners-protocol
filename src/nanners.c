//
//Created by Bananums: https://github.com/Bananums on 11/5/25.
//

#include "nanners/nanners.h"
#include "nanners/nanners_log.h"
#include <string.h>
#include <assert.h>

typedef struct {
    uint16_t data[256];
} Crc16Table;

static Crc16Table MakeTable() {
    const uint16_t polynomial = 0x1021u; //MSB-first
   Crc16Table table = {0};
   for (uint32_t i = 0; i < 256; ++i) {
       uint16_t crc = i << 8;

       for (int b = 0; b < 8; ++b) {
           const bool msb = (crc & 0x8000u) != 0u;
           crc = (uint16_t)(crc << 1);
           if (msb) {
               crc ^= polynomial;
           }
       }
       table.data[i] = crc;
   }
    return table;
}

static const Crc16Table* GetCrc16Table() {
    static Crc16Table table;
    static int initialized = 0;
    if (initialized == 0) {
        table = MakeTable();
        initialized = 1;
    }
    return &table;
}

static uint16_t ComputeCrc16(uint16_t crc, const uint8_t *data, uint8_t len) { //TODO(add into state machine)
    const Crc16Table* table = GetCrc16Table(); // Get from static storage to only compute once in program
    for (uint8_t i = 0; i < len; ++i) {
        const uint8_t index = crc >> 8 & 0xFFu ^ data[i];
        crc = (uint16_t)(crc << 8 ^ table->data[index]);
    }
    return crc;
}

static bool NannersCheckFrameCrc(const NannersFrame* frame) {
    assert(frame != NULL);
    assert(frame->length <= NANNERS_MAX_PAYLOAD_SIZE);

    const uint16_t wanted_crc = ComputeFrameCrc(frame);
    return (wanted_crc == frame->crc);
}

/* ---- Frame CRC over: frame_id, seq, length, payload[0..length-1] -------- */
/* MSB-first (network byte order) for frame_id */
uint16_t ComputeFrameCrc(const NannersFrame* frame) {
    assert(frame != NULL);
    assert(frame->length <= NANNERS_MAX_PAYLOAD_SIZE);

    //enum { kConst = sizeof(frame->frame_id) + sizeof(frame->seq) + sizeof(frame->length)};
    enum { kHeaderSize = 4 };  // frame_id(2) + seq(1) + length(1)

    uint8_t buffer[kHeaderSize];
    buffer[0] = (uint8_t)(frame->frame_id >> 8);
    buffer[1] = (uint8_t)(frame->frame_id & 0xFFu);
    buffer[2] = frame->seq;
    buffer[3] = frame->length;

    /* CRC-16/CCITT-FALSE: poly=0x1021, init=0xFFFF, xorout=0x0000, refin=false, refout=false */
    //Partially compute the crc over the header
    const uint16_t partial_computed_crc = ComputeCrc16(0xFFFFu, buffer, sizeof(buffer));

    // Complete CRC calculate on remaining payload. Done in two stages to reduce buffer size.
    return ComputeCrc16(partial_computed_crc, frame->payload, frame->length);
}

/* ---- Frame CRC over: frame_id, seq, length, payload[0..length-1] -------- */
/* MSB-first (network byte order) for frame_id */
uint16_t ComputeCrc(const uint16_t frame_id, const uint8_t seq,
                              const uint8_t* payload, const uint8_t length) {
    assert(length <= NANNERS_MAX_PAYLOAD_SIZE);

    //enum { kConst = sizeof(frame->frame_id) + sizeof(frame->seq) + sizeof(frame->length)};
    enum { kHeaderSize = 4 };  // frame_id(2) + seq(1) + length(1)

    uint8_t buffer[kHeaderSize];
    buffer[0] = (uint8_t)(frame_id >> 8);
    buffer[1] = (uint8_t)(frame_id & 0xFFu);
    buffer[2] = seq;
    buffer[3] = length;

    /* CRC-16/CCITT-FALSE: poly=0x1021, init=0xFFFF, xorout=0x0000, refin=false, refout=false */
    //Partially compute the crc over the header
    const uint16_t partial_computed_crc = ComputeCrc16(0xFFFFu, buffer, sizeof(buffer));

    // Complete CRC calculate on remaining payload. Done in two stages to reduce buffer size.
    return ComputeCrc16(partial_computed_crc, payload, length);
}

void NannersInit(NannersFrame* frame){
    NannersReset(frame);
}

void NannersReset(NannersFrame* frame) {
    frame->state = NANNERS_WAIT_FOR_SOF;
    frame->frame_id = 0;
    frame->length = 0;
    frame->crc = 0;
    frame->index = 0;
    //frame->payload left as is, will be overwritten by new bytes
}

// Process each byte from UART
NannersResult NannersProcessByte(NannersFrame* frame, const uint8_t byte, NannersStats *stats) {
    NannersResult result = NANNERS_IN_PROGRESS;

    switch (frame->state) {
        case NANNERS_WAIT_FOR_SOF:
            if (byte == (uint8_t)NANNERS_START_OF_FRAME) { // Start of Frame
              NANNERS_LOG("Received start of frame byte: %02X\n", byte);
              frame->state = NANNERS_READ_FRAME_ID;
              frame->index = 0;
            } else {
              result = NANNERS_WAIT;
            }
        break;

        case NANNERS_READ_FRAME_ID:
            if (frame->index == 0) {
                frame->frame_id = (uint16_t)byte << 8; // high byte first
                frame->index = 1;
                NANNERS_LOG("Received frame id part HI: %02X\n", byte);
            } else {  // index == 1
                frame->frame_id |= (uint16_t)byte; // low byte last
                NANNERS_LOG("Received frame id part LO: %02X\n", byte);
                NANNERS_LOG("Received frame id 0x%04X - %u\n", frame->frame_id, frame->frame_id);
                frame->state = NANNERS_READ_SEQUENCE;
                frame->index = 0;
            }
        break;

        case NANNERS_READ_SEQUENCE: {
            frame->seq = byte;
            NANNERS_LOG("Received frame seq 0x%04X - %u\n", frame->seq, frame->seq);
            frame->state = NANNERS_READ_LENGTH;
            frame->index = 0;
            break;
        }

        case NANNERS_READ_LENGTH:
            frame->length = byte;
            NANNERS_LOG("Received frame length %d\n", frame->length);
        if (frame->length > NANNERS_MAX_PAYLOAD_SIZE) {
            result = NANNERS_PROTOCOL_ERROR;
            if (stats != NULL) {
                ++stats->format_fail;
                ++stats->resync_count;
            }
            frame->state = NANNERS_WAIT_FOR_SOF; // Invalid length, reset

        } else {
            frame->state = (frame->length == 0) ? NANNERS_READ_CRC : NANNERS_READ_PAYLOAD; // Ternary expression to avoid branching
            frame->state = NANNERS_READ_PAYLOAD;
            frame->index = 0;
        }
        break;

        case NANNERS_READ_PAYLOAD:
            frame->payload[frame->index++] = byte;
        if (frame->index >= frame->length) {
            frame->state = NANNERS_READ_CRC;
            frame->index = 0;
        }
        break;

        case NANNERS_READ_CRC:
            if (frame->index == 0) {
                frame->crc = (uint16_t)byte << 8; // high byte first
                frame->index = 1;
                NANNERS_LOG("Received crd part HI: %02X\n", byte);
            } else {  // index == 1
                frame->crc |= (uint16_t)byte; // low byte last
                NANNERS_LOG("Received crc part LO: %02X\n", byte);
                NANNERS_LOG("Received CRC 0x%04X - %u\n", frame->crc, frame->crc);
                frame->state = NANNERS_VERIFY_EOF;
                frame->index = 0;
            }
        break;

        case NANNERS_VERIFY_EOF:
            if (byte == (uint8_t)NANNERS_END_OF_FRAME) { // End of Frame
                NANNERS_LOG("Received end of frame byte %02X\n", byte);
                if (NannersCheckFrameCrc(frame)) {
                    NANNERS_LOG("CRC okay\n");
                    if (stats != NULL) {
                        ++stats->frames_ok;
                    }
                    result = NANNERS_FRAME_READY;
                } else {
                    NANNERS_LOG("CRC failed\n");
                    if (stats != NULL) {
                        ++stats->crc_fail;
                        ++stats->resync_count;
                    }
                    result = NANNERS_CRC_FAIL;
                }
            } else {
                if (stats != NULL) {
                    ++stats->format_fail;
                    ++stats->resync_count;

                }
                result = NANNERS_PROTOCOL_ERROR;
            }
        frame->state = NANNERS_WAIT_FOR_SOF;
        break;

        default:
            frame->state = NANNERS_WAIT_FOR_SOF;
        break;
    }
    return result;
}

int32_t NannersSerializeFrame(const uint16_t frame_id, const uint8_t seq,
                              const uint8_t* payload, const uint8_t length,
                              uint8_t* out_wire, size_t out_wire_len) {
    if (out_wire == NULL) {
        return -1;
    }

    if (length > NANNERS_MAX_PAYLOAD_SIZE) {
        return -1;
    }

    if (sizeof(*payload) > NANNERS_MAX_PAYLOAD_SIZE) {
        return -1;
    }

    const size_t wire_len =
        1                       // SOF      (uint8)
      + 2                       // Frame ID (uint16)
      + 1                       // Sequence (uint8)
      + 1                       // length   (uint8)
      + (size_t)length          // payload  [1-8]
      + 2                       // CRC      (uint16)
      + 1;                      // EOF      (uint8)

    if (out_wire_len < wire_len) {
        return -1;
    }

    size_t n = 0;
    out_wire[n++] = (uint8_t)NANNERS_START_OF_FRAME;
    out_wire[n++] = (uint8_t)(frame_id >> 8);
    out_wire[n++] = (uint8_t)(frame_id & 0xFF);
    out_wire[n++] = (uint8_t)(seq);
    out_wire[n++] = (uint8_t)(length);

    if (length > 0) {
        memcpy(&out_wire[n], payload, length); //Copy payload from n to n+frame.length
        n += length;
    }

    const uint16_t crc = ComputeCrc(frame_id, seq, payload, length);
    out_wire[n++] = (uint8_t)(crc >> 8);
    out_wire[n++] = (uint8_t)(crc & 0xFF);
    out_wire[n++] = (uint8_t)(NANNERS_END_OF_FRAME);

    return (int32_t)(n);
}
