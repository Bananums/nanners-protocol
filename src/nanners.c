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

static uint16_t ComputeCrc16(const uint8_t *data, uint8_t len) { //TODO(add into state machine)
    const Crc16Table* table = GetCrc16Table(); // Get from static storage to only compute once in program
    uint16_t crc = 0xFFFFu; // Final XOR = 0 init value
    for (uint8_t i = 0; i < len; ++i) {
        const uint8_t index = crc >> 8 ^ data[i] & 0xFFu;
        crc = (uint16_t)(crc << 8 ^ table->data[index]);
    }
    return crc;
}

/* ---- Frame CRC over: frame_id, seq, length, payload[0..length-1] -------- */
/* MSB-first (network byte order) for frame_id */
uint16_t ComputeFrameCrc(const NannersFrame* frame) {
    assert(frame != NULL);
    assert(frame->length <= NANNERS_MAX_PAYLOAD_SIZE);

    enum { kConst = sizeof(frame->frame_id) + sizeof(frame->seq) + sizeof(frame->length) };
    uint8_t buffer[kConst + sizeof(frame->payload)];

    buffer[0] = (uint8_t)(frame->frame_id >> 8);
    buffer[1] = (uint8_t)(frame->frame_id & 0xFFu);
    buffer[2] = frame->seq;
    buffer[3] = frame->length;
    for (uint8_t i = 0; i < frame->length; ++i) {
        buffer[kConst + i] = frame->payload[i];
    }
    const uint8_t total = (uint8_t)kConst + (uint8_t)frame->length;
    return ComputeCrc16(buffer, total);

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
                //if (validate_crc(frame->payload, frame->length, frame->crc)) { //TODO add check
                    //printf("Valid frame\n");
                    frame->valid = true;
                //}
            }
        frame->state = NANNERS_WAIT_FOR_SOF;
        break;

        default:
            frame->state = NANNERS_WAIT_FOR_SOF;
        break;
    }
}
