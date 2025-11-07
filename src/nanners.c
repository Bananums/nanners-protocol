//
//Created by Bananums: https://github.com/Bananums on 11/5/25.
//

#include "nanners/nanners.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

static uint16_t NannersCalculateCrc16(const uint8_t *data, uint8_t len) { //TODO(add into state machine)
    /* TODO: polynomial/seed per spec */
    uint16_t crc = 0xFFFFu;
    for (uint8_t i = 0; i < len; ++i) {
        crc ^= (uint16_t)data[i];
        for (int b = 0; b < 8; ++b) {
            crc = (crc & 1u) ? (crc >> 1) ^ 0xA001u : (crc >> 1);
        }
    }
    return crc;
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
