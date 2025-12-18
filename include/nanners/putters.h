//
// Created by Bananums: https://github.com/Bananums on 12/18/25.
//

#ifndef NANNERS_PROTOCOL_INCLUDE_NANNERS_PUTTERS_H
#define NANNERS_PROTOCOL_INCLUDE_NANNERS_PUTTERS_H

#include <stdint.h>

/**
 * @file putters.h
 * @brief Helpers for serializing values into a Nanners payload buffer.
 *
 * These functions append values to a byte buffer (`payload`) at the current
 * offset (`*len`) and increment `*len` by the number of bytes written.
 *
 * All multibyte values are written in big-endian (network byte order) when
 * the function name ends with `BE`.
 *
 * @note The caller must provide a valid payload buffer of size
 *       NANNERS_MAX_PAYLOAD_SIZE bytes.
 * @note On success, `*len` is advanced. On failure, the buffer and `*len`
 *       are not modified.
 */

/**
 * @brief Append a boolean value to the payload.
 *
 * Encodes the value as a single byte: 0 for false, 1 for true.
 *
 * @param payload  Destination buffer.
 * @param len      In/out: current payload length in bytes.
 * @param value    Boolean value to append.
 * @return true if the value was written; false if inputs are invalid or there
 *         is insufficient space.
 */
bool NannersPutBoolean(uint8_t* payload, uint8_t* len, bool value);

/**
 * @brief Append an unsigned 8-bit value to the payload.
 *
 * @param payload  Destination buffer.
 * @param len      In/out: current payload length in bytes.
 * @param value    Value to append.
 * @return true on success; false on invalid inputs or insufficient space.
 */
bool NannersPutU8(uint8_t* payload, uint8_t* len, uint8_t value);

/**
 * @brief Append an unsigned 16-bit value to the payload in big-endian order.
 *
 * @param payload  Destination buffer.
 * @param len      In/out: current payload length in bytes.
 * @param value    Value to append.
 * @return true on success; false on invalid inputs or insufficient space.
 */
bool NannersPutU16BE(uint8_t* payload, uint8_t* len, uint16_t value);

/**
 * @brief Append an unsigned 32-bit value to the payload in big-endian order.
 *
 * @param payload  Destination buffer.
 * @param len      In/out: current payload length in bytes.
 * @param value    Value to append.
 * @return true on success; false on invalid inputs or insufficient space.
 */
bool NannersPutU32BE(uint8_t* payload, uint8_t* len, uint32_t value);

/**
 * @brief Append a signed 8-bit value to the payload.
 *
 * The value is encoded as two's complement in a single byte.
 *
 * @param payload  Destination buffer.
 * @param len      In/out: current payload length in bytes.
 * @param value    Value to append.
 * @return true on success; false on invalid inputs or insufficient space.
 */
bool NannersPutS8(uint8_t* payload, uint8_t* len, int8_t value);

/**
 * @brief Append a signed 16-bit value to the payload in big-endian order.
 *
 * The value is encoded as two's complement.
 *
 * @param payload  Destination buffer.
 * @param len      In/out: current payload length in bytes.
 * @param value    Value to append.
 * @return true on success; false on invalid inputs or insufficient space.
 */
bool NannersPutS16BE(uint8_t* payload, uint8_t* len, int16_t value);

/**
 * @brief Append a signed 32-bit value to the payload in big-endian order.
 *
 * The value is encoded as two's complement.
 *
 * @param payload  Destination buffer.
 * @param len      In/out: current payload length in bytes.
 * @param value    Value to append.
 * @return true on success; false on invalid inputs or insufficient space.
 */
bool NannersPutS32BE(uint8_t* payload, uint8_t* len, int32_t value);

/**
 * @brief Append a 32-bit float to the payload in big-endian order.
 *
 * The float is serialized by copying its IEEE-754 binary32 bit pattern into
 * a 32-bit integer and writing that integer in big-endian order.
 *
 * @param payload  Destination buffer.
 * @param len      In/out: current payload length in bytes.
 * @param value    Float value to append.
 * @return true on success; false on invalid inputs, insufficient space, or if
 *         `sizeof(float) != 4`.
 */
bool NannersPutF32BE(uint8_t* payload, uint8_t* len, float value);

#endif //NANNERS_PROTOCOL_INCLUDE_NANNERS_PUTTERS_H
