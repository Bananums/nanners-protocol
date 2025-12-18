//
// Created by Bananums: https://github.com/Bananums on 12/18/25.
//

#include "nanners/logs.h"

const char* NannersPutResultToString(const NannersPutResult result) {
    switch (result) {
        case NANNERS_PUT_OK:
            return "OK";
        case NANNERS_PUT_ERR_PAYLOAD_NULL:
            return "Payload pointer is NULL";
        case NANNERS_PUT_ERR_LENGTH_NULL:
            return "Length pointer is NULL";
        case NANNERS_PUT_ERR_OUT_OF_BOUNDS:
            return "Payload out of bounds";
        case NANNERS_PUT_ERR_UNSUPPORTED_TYPE:
            return "Unsupported type";
        default:
            return "Unknown NannersPutResult";
    }
}
