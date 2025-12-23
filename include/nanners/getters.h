//
// Created by Bananums: https://github.com/Bananums on 12/18/25.
//

#ifndef NANNERS_PROTOCOL_INCLUDE_NANNERS_GETTERS_H
#define NANNERS_PROTOCOL_INCLUDE_NANNERS_GETTERS_H

#include <stdint.h>
#include <stdbool.h>

#include "nanners/enums.h"

#ifdef __cplusplus
extern "C" {
#endif

NannersCodecResult NannersGetBoolean(const uint8_t* payload, uint8_t payload_len, uint8_t* pos,  bool* out);

NannersCodecResult NannersGetU8(const uint8_t* payload, uint8_t payload_len, uint8_t* pos, uint8_t* out);

NannersCodecResult NannersGetU16BE(const uint8_t* payload, uint8_t payload_len, uint8_t* pos, uint16_t* out);

NannersCodecResult NannersGetU32BE(const uint8_t* payload, uint8_t payload_len, uint8_t* pos, uint32_t* out);

NannersCodecResult NannersGetS8(const uint8_t* payload, uint8_t payload_len, uint8_t* pos,  int8_t* out);

NannersCodecResult NannersGetS16BE(const uint8_t* payload, uint8_t payload_len, uint8_t* pos,  int16_t* out);

NannersCodecResult NannersGetS32BE(const uint8_t* payload, uint8_t payload_len, uint8_t* pos,  int32_t* out);

NannersCodecResult NannersGetF32BE(const uint8_t* payload, uint8_t payload_len, uint8_t* pos, float* out);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif //NANNERS_PROTOCOL_INCLUDE_NANNERS_GETTERS_H
