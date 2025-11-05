//
//Created by Bananums: https://github.com/Bananums on 11/5/25.
//

#include "nanners/nanners.h"
#include <string.h>
#include <stdio.h>

static NannersFrame frame = {
    .state = NANNERS_WAIT_FOR_SOF,
    .frame_id = 0,
    .length = 0,
    .payload = {0},
    .crc = 0,
    .index = 0,
    .valid = false,
};

static uint16_t NannersCalculateCrc16(const uint8_t *data, uint8_t len) {
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

/* Big-endian assembly helpers for 16-bit fields */
static inline void be16_push(uint16_t v, uint8_t *hi, uint8_t *lo) {
    *hi = (uint8_t)(v >> 8);
    *lo = (uint8_t)(v & 0xFF);
}
static inline uint16_t be16_pull(uint8_t hi, uint8_t lo) {
    return (uint16_t)((uint16_t)hi << 8 | (uint16_t)lo);
}

static void NannersResetToSof(void) {
    frame.state = NANNERS_WAIT_FOR_SOF;
    frame.index = 0;
    frame.length = 0;
    frame.valid = false;
}

// Process each byte from UART
void NannersProcessBytes(uint8_t byte) {

    switch (frame.state) {
        case NANNERS_WAIT_FOR_SOF:
            if (byte == (uint8_t)NANNERS_START_OF_FRAME) { // Start of Frame
              printf("Received start of frame byte: %02X\n", byte);
              frame.state = NANNERS_READ_FRAME_ID;
              frame.index = 0;
            }
        break;

        case NANNERS_READ_FRAME_ID:
            //printf("Received frame id byte: %02X\n", byte);
            if (frame.index == 0) {
                ((uint8_t*)&frame.frame_id)[1] = byte;
                frame.index++;
                printf("Received frame id part byte: %02X\n", byte); //TODO(look into handling little and big endian)
            }
            else if (frame.index == 1) {
                printf("Received frame id part byte: %02X\n", byte);
                ((uint8_t*)&frame.frame_id)[0] = byte;
              printf("Received frame id %d\n", frame.frame_id);
              frame.state = NANNERS_READ_LENGTH;
              frame.index = 0;
            }
        break;

        case NANNERS_READ_LENGTH:
            frame.length = byte;
            printf("Received frame length %d\n", frame.length);
        if (frame.length > NANNERS_MAX_PAYLOAD_SIZE) {
            frame.state = NANNERS_WAIT_FOR_SOF; // Invalid length, reset
        } else {
            frame.state = NANNERS_READ_PAYLOAD;
            frame.index = 0;
        }
        break;

        case NANNERS_READ_PAYLOAD:
            frame.payload[frame.index++] = byte;
            //printf("Received frame payload %02X\n", byte);
        if (frame.index >= frame.length) {
            frame.state = NANNERS_READ_CRC;
            frame.index = 0;
        }
        break;

        case NANNERS_READ_CRC:
            ((uint8_t*)&frame.crc)[frame.index++] = byte;
            //printf("Received CRC byte %02X\n", byte);
        if (frame.index == 2) {
              printf("Received CRC %u\n", frame.crc);
            frame.state = NANNERS_VERIFY_EOF;
            frame.index = 0; //TODO Check if necessray to reset here
        }
        break;

        case NANNERS_VERIFY_EOF:
            if (byte == (uint8_t)NANNERS_END_OF_FRAME) { // End of Frame
                printf("Received end of frame byte %02X\n", byte);
                //if (validate_crc(frame.payload, frame.length, frame.crc)) { //TODO add check
                    //printf("Valid frame\n");
                    frame.valid = true;
                //}
            }
        frame.state = NANNERS_WAIT_FOR_SOF;
        break;

        default:
            frame.state = NANNERS_WAIT_FOR_SOF;
        break;
    }
}

bool NannersGetFrame(NannersFrame *frame_out) {
  if(!frame.valid) {
    return false;
  }

  frame_out->frame_id = frame.frame_id;
  frame_out->length = frame.length;
  memcpy(frame_out->payload, frame.payload, frame.length);

  frame.valid = false;
  return true;
}