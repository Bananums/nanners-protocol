//Created by Bananums: https://github.com/Bananums

#ifndef NANNERS_PROTOCOL_INCLUDE_NANNERS_ENUMS_H
#define NANNERS_PROTOCOL_INCLUDE_NANNERS_ENUMS_H

#ifdef __cplusplus
extern "C" {
#endif

enum {
    NANNERS_MAX_PAYLOAD_SIZE = 8,
};

enum {
    NANNERS_START_OF_FRAME = 0xAA,
    NANNERS_END_OF_FRAME = 0x55
};

typedef enum {
    NANNERS_WAIT_FOR_SOF = 0,
    NANNERS_READ_FRAME_ID = 1,
    NANNERS_READ_SEQUENCE = 2,
    NANNERS_READ_LENGTH = 3,
    NANNERS_READ_PAYLOAD = 4,
    NANNERS_READ_CRC = 5,
    NANNERS_VERIFY_EOF = 6
} NannersState;

typedef enum {
    NANNERS_WAIT = 0,
    NANNERS_IN_PROGRESS = 1,
    NANNERS_FRAME_READY = 2,
    NANNERS_CRC_FAIL = 3,
    NANNERS_PROTOCOL_ERROR = 4
} NannersResult;

typedef enum {
    NANNERS_PUT_OK = 0,
    NANNERS_PUT_ERR_PAYLOAD_NULL,
    NANNERS_PUT_ERR_LENGTH_NULL,
    NANNERS_PUT_ERR_OUT_OF_BOUNDS,
    NANNERS_PUT_ERR_UNSUPPORTED_TYPE
} NannersPutResult;

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif //NANNERS_PROTOCOL_INCLUDE_NANNERS_ENUMS_H
