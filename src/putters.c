//
// Created by Bananums: https://github.com/Bananums on 12/18/25.
//

#include  <string.h>

#include "nanners/putters.h"
#include "nanners/enums.h"

static bool ValidInput(const uint8_t* payload, const uint8_t* len, uint8_t bytes){
  if(payload == NULL || len == NULL){
    return false;
  }

  if(*len > NANNERS_MAX_PAYLOAD_SIZE){
    return false;
  }

  return bytes <= (uint8_t)(NANNERS_MAX_PAYLOAD_SIZE - *len);
}

bool NannersPutBoolean(uint8_t* payload, uint8_t* len, bool value){
  const uint8_t bytes = 1u;
  if(!ValidInput(payload, len, bytes)){
    return false;
  }

  payload[(*len)++] = value ? 1u : 0u;

  return true;
}

bool NannersPutU8(uint8_t* payload, uint8_t* len, uint8_t value){
  const uint8_t bytes = 1u;
 if(!ValidInput(payload, len, bytes)){
   return false;
 }

 payload[(*len)++] = value;

 return true;
};

bool NannersPutU16BE(uint8_t* payload, uint8_t* len, uint16_t value){
  const uint8_t bytes = 2u;
  if(!ValidInput(payload, len, bytes)){
    return false;
  }

  payload[(*len)++] = (uint8_t)((value >> 8) & 0xFFu);
  payload[(*len)++] = (uint8_t)(value & 0xFFu);

  return true;
}

bool NannersPutU32BE(uint8_t* payload, uint8_t* len, uint32_t value){
  const uint8_t bytes = 4u;
  if(!ValidInput(payload, len, bytes)){
    return false;
  }

  payload[(*len)++] = (uint8_t)((value >> 24) & 0xFFu);
  payload[(*len)++] = (uint8_t)((value >> 16) & 0xFFu);
  payload[(*len)++] = (uint8_t)((value >> 8) & 0xFFu);
  payload[(*len)++] = (uint8_t)(value & 0xFFu);

  return true;
}

bool NannersPutS8(uint8_t* payload, uint8_t* len, int8_t value){
  return NannersPutU8(payload, len, (uint8_t)value);
}

bool NannersPutS16BE(uint8_t* payload, uint8_t* len, int16_t value){
  return NannersPutU16BE(payload, len, (uint16_t)value);
}

bool NannersPutS32BE(uint8_t* payload, uint8_t* len, int32_t value){
  return NannersPutU32BE(payload, len, (uint32_t)value);
}

bool NannersPutF32BE(uint8_t* payload, uint8_t* len, float value){
  if(sizeof(float) != 4u){return false;} //Reqires 32-bit float. Should not be an issue on "todays" devices.
  const uint8_t bytes = 4u;
  if(!ValidInput(payload, len, bytes)){
    return false;
  }

  uint32_t temp = 0u;
  memcpy(&temp, &value, sizeof(temp));

  return NannersPutU32BE(payload, len, temp);
}
