//
// Created by Bananums: https://github.com/Bananums on 12/18/25.
//

#ifndef NANNERS_PROTOCOL_INCLUDE_NANNERS_PUTTERS_H
#define NANNERS_PROTOCOL_INCLUDE_NANNERS_PUTTERS_H

#include <stdint.h>

bool NannersPutBoolean(uint8_t* payload, uint8_t* len, bool value);
/*!
 Puts a uint8 (1 byte) into the Nanners Payload
 */
bool NannersPutU8(uint8_t* payload, uint8_t* len, uint8_t value);

bool NannersPutU16BE(uint8_t* payload, uint8_t* len, uint16_t value);
bool NannersPutU32BE(uint8_t* payload, uint8_t* len, uint32_t value);

/*!
 Puts a int8 (1 byte) into the Nanners Payload
 */
bool NannersPutS8(uint8_t* payload, uint8_t* len, int8_t value);

/*!
 Puts a int16 (2 byte) into the Nanners Payload
 */
bool NannersPutS16BE(uint8_t* payload, uint8_t* len, int16_t value);

bool NannersPutS32BE(uint8_t* payload, uint8_t* len, int32_t value);

/*!
 Puts a float (4 bytes) into the Nanners Payload
 Big endian format
 assumes IEEE-754 float
 */
bool NannersPutF32BE(uint8_t* payload, uint8_t* len, float value);

#endif //NANNERS_PROTOCOL_INCLUDE_NANNERS_PUTTERS_H
