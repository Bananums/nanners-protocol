//
// Created by Bananums: https://github.com/Bananums on 12/18/25.
//

#include  <string.h>

#include "nanners/putters.h"
#include "nanners/enums.h"

static NannersPutResult ValidInput(const uint8_t* payload, const uint8_t* len, uint8_t bytes){
  if(payload == NULL){
    return NANNERS_PUT_ERR_PAYLOAD_NULL ;
  }

  if(len == NULL){
    return NANNERS_PUT_ERR_LENGTH_NULL;
  }

  if(*len > NANNERS_MAX_PAYLOAD_SIZE){
    return NANNERS_PUT_ERR_OUT_OF_BOUNDS;
  }

  if (bytes > (uint8_t)(NANNERS_MAX_PAYLOAD_SIZE - *len)) {
    return NANNERS_PUT_ERR_OUT_OF_BOUNDS;
  }

  return NANNERS_PUT_OK;
}

NannersPutResult NannersPutBoolean(uint8_t* payload, uint8_t* len, bool value){
  const uint8_t bytes = 1u;
  NannersPutResult result = ValidInput(payload, len, bytes);
  if(result != NANNERS_PUT_OK){
    return result;
  }

  payload[(*len)++] = value ? 1u : 0u;

  return NANNERS_PUT_OK;
}

NannersPutResult NannersPutU8(uint8_t* payload, uint8_t* len, uint8_t value){
  const uint8_t bytes = 1u;
  NannersPutResult result = ValidInput(payload, len, bytes);
  if(result != NANNERS_PUT_OK){
    return result;
  }

 payload[(*len)++] = value;

 return NANNERS_PUT_OK;
}

NannersPutResult NannersPutU16BE(uint8_t* payload, uint8_t* len, uint16_t value){
  const uint8_t bytes = 2u;
  NannersPutResult result = ValidInput(payload, len, bytes);
  if(result != NANNERS_PUT_OK){
    return result;
  }

  payload[(*len)++] = (uint8_t)((value >> 8) & 0xFFu);
  payload[(*len)++] = (uint8_t)(value & 0xFFu);

  return NANNERS_PUT_OK;
}

NannersPutResult NannersPutU32BE(uint8_t* payload, uint8_t* len, uint32_t value){
  const uint8_t bytes = 4u;
  NannersPutResult result = ValidInput(payload, len, bytes);
  if(result != NANNERS_PUT_OK){
    return result;
  }

  payload[(*len)++] = (uint8_t)((value >> 24) & 0xFFu);
  payload[(*len)++] = (uint8_t)((value >> 16) & 0xFFu);
  payload[(*len)++] = (uint8_t)((value >> 8) & 0xFFu);
  payload[(*len)++] = (uint8_t)(value & 0xFFu);

  return NANNERS_PUT_OK;
}

NannersPutResult NannersPutS8(uint8_t* payload, uint8_t* len, int8_t value){
  return NannersPutU8(payload, len, (uint8_t)value);
}

NannersPutResult NannersPutS16BE(uint8_t* payload, uint8_t* len, int16_t value){
  return NannersPutU16BE(payload, len, (uint16_t)value);
}

NannersPutResult NannersPutS32BE(uint8_t* payload, uint8_t* len, int32_t value){
  return NannersPutU32BE(payload, len, (uint32_t)value);
}

NannersPutResult NannersPutF32BE(uint8_t* payload, uint8_t* len, float value){
  if(sizeof(float) != 4u){ //Reqires 32-bit float. Should not be an issue on "todays" devices.
    return NANNERS_PUT_ERR_UNSUPPORTED_TYPE;
  }

  uint32_t temp = 0u;
  memcpy(&temp, &value, sizeof(temp));

  return NannersPutU32BE(payload, len, temp);
}
