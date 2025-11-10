//
//Created by Bananums: https://github.com/Bananums on 11/5/25.
//

#include "nanners/nanners.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

typedef struct {
    uint16_t data[256];
} Crc16Table;

static Crc16Table MakeTable() {
    const uint16_t polynomial = 0x1021u; //MSB-first
   Crc16Table table = {0};
   for (uint32_t i = 0; i < 256; ++i) {
       uint16_t crc = (uint16_t)(i << 8);

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

static bool NannersCheckCrc(const NannersFrame* frame) {
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

void NannersInit(NannersFrame* frame){
    NannersReset(frame);
}

void NannersReset(NannersFrame* frame) {
    frame->state = NANNERS_WAIT_FOR_SOF;
    frame->frame_id = 0;
    frame->length = 0;
    frame->crc = 0;
    frame->valid = false;
    //frame->payload left as is, will be overwritten by new bytes
}

// Process each byte from UART
void NannersProcessBytes(NannersFrame* frame, uint8_t byte) {

    switch (frame->state) {
        case NANNERS_WAIT_FOR_SOF:
            if (byte == (uint8_t)NANNERS_START_OF_FRAME) { // Start of Frame
              printf("Received start of frame byte: %02X\n", byte);
              frame->state = NANNERS_READ_FRAME_ID;
              frame->index = 0;
            }
        break;

        case NANNERS_READ_FRAME_ID:
            if (frame->index == 0) {
                frame->frame_id = (uint16_t)byte << 8; // high byte first
                frame->index = 1;
                printf("Received frame id part HI: %02X\n", byte);
            } else {  // index == 1
                frame->frame_id |= (uint16_t)byte; // low byte last
                printf("Received frame id part LO: %02X\n", byte);
                printf("Received frame id 0x%04X - %u\n", frame->frame_id, frame->frame_id);
                frame->state = NANNERS_READ_SEQUENCE;
                frame->index = 0;
            }
        break;

        case NANNERS_READ_SEQUENCE: {
            frame->seq = byte;
            printf("Received frame seq 0x%04X - %u\n", frame->seq, frame->seq);
            frame->state = NANNERS_READ_LENGTH;
            frame->index = 0;
            break;
        }

        case NANNERS_READ_LENGTH:
            frame->length = byte;
            printf("Received frame length %d\n", frame->length);
        if (frame->length > NANNERS_MAX_PAYLOAD_SIZE) {
            frame->state = NANNERS_WAIT_FOR_SOF; // Invalid length, reset
        } else {
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
                printf("Received crd part HI: %02X\n", byte);
            } else {  // index == 1
                frame->crc |= (uint16_t)byte; // low byte last
                printf("Received crc part LO: %02X\n", byte);
                printf("Received CRC 0x%04X - %u\n", frame->crc, frame->crc);
                frame->state = NANNERS_VERIFY_EOF;
                frame->index = 0; //TODO Check if necessray to reset here
            }
        break;

        case NANNERS_VERIFY_EOF:
            if (byte == (uint8_t)NANNERS_END_OF_FRAME) { // End of Frame
                printf("Received end of frame byte %02X\n", byte);
                if (NannersCheckCrc(frame)) {
                    printf("CRC okay\n");
                    frame->valid = true;
                } else {
                    printf("CRC failed\n");
                }
            }
        frame->state = NANNERS_WAIT_FOR_SOF;
        break;

        default:
            frame->state = NANNERS_WAIT_FOR_SOF;
        break;
    }
}
