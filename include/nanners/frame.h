//Created by Bananums: https://github.com/Bananums

#ifndef NANNERS_PROTOCOL_INCLUDE_NANNERS_FRAME_H
#define NANNERS_PROTOCOL_INCLUDE_NANNERS_FRAME_H

#include "enums.h"
#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

// UART Frame Structure (Used for Transmission)
typedef struct __attribute__((packed)) {
    NannersState state;
    uint16_t frame_id;
    uint8_t seq;
    uint8_t length;
    uint8_t payload[NANNERS_MAX_PAYLOAD_SIZE];
    uint16_t crc;
    uint8_t index;
} NannersFrame;

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif //NANNERS_PROTOCOL_INCLUDE_NANNERS_FRAME_H
