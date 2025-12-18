//
// Created by Bananums: https://github.com/Bananums on 12/18/25.
//

#ifndef NANNERS_PROTOCOL_INCLUDE_NANNERS_GETTERS_H
#define NANNERS_PROTOCOL_INCLUDE_NANNERS_GETTERS_H

#include <stdint.h>


bool NannersGetU8(const uint8_t* payload, uint8_t payload_len,
                              uint8_t* pos, uint8_t* out){return true;}

bool NannersGetU16BE(const uint8_t* payload, uint8_t payload_len,
                                 uint8_t* pos, uint16_t* out){return true;}

bool NannersGetU32BE(const uint8_t* payload, uint8_t payload_len,
                                 uint8_t* pos, uint32_t* out){return true;}

bool NannersGetF32BE(const uint8_t* payload, uint8_t payload_len,
                                 uint8_t* pos, float* out){return true;}


#endif //NANNERS_PROTOCOL_INCLUDE_NANNERS_GETTERS_H
