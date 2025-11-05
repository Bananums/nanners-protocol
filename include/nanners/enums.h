//Created by Bananums: https://github.com/Bananums

#ifndef NANNERS_PROTOCOL_SRC_ENUMS_H
#define NANNERS_PROTOCOL_SRC_ENUMS_H

#ifdef __cplusplus
extern "C" {
#endif

enum {
    NANNERS_MAX_PAYLOAD_SIZE = 16,
};

enum {
    NANNERS_START_OF_FRAME = 0xAA,
    NANNERS_END_OF_FRAME = 0x55
};

typedef enum {
    NANNERS_WAIT_FOR_SOF  = 0,
    NANNERS_READ_FRAME_ID = 1,
    NANNERS_READ_LENGTH   = 2,
    NANNERS_READ_PAYLOAD  = 3,
    NANNERS_READ_CRC      = 4,
    NANNERS_VERIFY_EOF    = 5
} NannersState;

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif // NANNERS_PROTOCOL_SRC_ENUMS_H
