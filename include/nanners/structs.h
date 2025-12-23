//
// Created by Bananums: https://github.com/Bananums on 12/17/25.
//

#ifndef NANNERS_PROTOCOL_INCLUDE_NANNERS_STRUCTS_H
#define NANNERS_PROTOCOL_INCLUDE_NANNERS_STRUCTS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t frames_ok; // How many valid frames have been retrieved.
    uint32_t crc_fail; // How many CRC fails has occurred.
    uint32_t resync_count; // Any bad length, bad EOF or CRC fail.
    uint32_t format_fail; // Any invalid frame structures.
} NannersStats ;

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif //NANNERS_PROTOCOL_INCLUDE_NANNERS_STRUCTS_H
