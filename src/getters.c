//
// Created by snackers on 12/23/25.
//

#include <string.h>

#include "nanners/getters.h"

static NannersCodecResult ValidGetInput(const uint8_t* payload, const uint8_t len, const uint8_t* pos, const uint8_t bytes){
  if(payload == NULL){
    return NANNERS_CODEC_ERR_PAYLOAD_NULL;
  }

  if(pos == NULL){
    return NANNERS_CODEC_ERR_POSITION_NULL;
  }

  if(*pos > len){
    return NANNERS_CODEC_ERR_OUT_OF_BOUNDS;
  }

  if((uint8_t)(len - *pos) < bytes){
    return NANNERS_CODEC_ERR_OUT_OF_BOUNDS;
  }

  return NANNERS_CODEC_OK;
}

NannersCodecResult NannersGetBoolean(const uint8_t* payload, uint8_t payload_len, uint8_t* pos,  bool* out){
  if(out == NULL){
    return NANNERS_CODEC_ERR_OUT_NULL;
  }

  uint8_t bytes = 1u;
  const NannersCodecResult result = ValidGetInput(payload, payload_len, pos, bytes);
  if(result != NANNERS_CODEC_OK){
    return result;
  }

  *out = (payload[(*pos)++]) ? 1u : 0u;

  return NANNERS_CODEC_OK;
}

NannersCodecResult NannersGetU8(const uint8_t* payload, uint8_t payload_len, uint8_t* pos, uint8_t* out){
  if(out == NULL){
    return NANNERS_CODEC_ERR_OUT_NULL;
  }

  uint8_t bytes = 1u;
  const NannersCodecResult result = ValidGetInput(payload, payload_len, pos, bytes);
  if(result != NANNERS_CODEC_OK){
    return result;
  }

  *out = (uint8_t)(payload[(*pos)++]);

  return NANNERS_CODEC_OK;
}

NannersCodecResult NannersGetU16BE(const uint8_t* payload, uint8_t payload_len, uint8_t* pos, uint16_t* out){

  if(out == NULL){
    return NANNERS_CODEC_ERR_OUT_NULL;
  }

  uint8_t bytes = 2u;
  const NannersCodecResult result = ValidGetInput(payload, payload_len, pos, bytes);
  if(result != NANNERS_CODEC_OK){
    return result;
  }

  *out = (uint16_t)((uint16_t)(payload[(*pos)++] << 8));
  *out |= (uint16_t)(payload[(*pos)++]);

  return NANNERS_CODEC_OK;
}

NannersCodecResult NannersGetU32BE(const uint8_t* payload, uint8_t payload_len, uint8_t* pos, uint32_t* out){

  return NANNERS_CODEC_OK;
}

NannersCodecResult NannersGetS8(const uint8_t* payload, uint8_t payload_len, uint8_t* pos,  int8_t* out){

  return NANNERS_CODEC_OK;
}

NannersCodecResult NannersGetS16BE(const uint8_t* payload, uint8_t payload_len, uint8_t* pos,  int16_t* out){

  return NANNERS_CODEC_OK;
}

NannersCodecResult NannersGetS32BE(const uint8_t* payload, uint8_t payload_len, uint8_t* pos,  int32_t* out){

  return NANNERS_CODEC_OK;
}

NannersCodecResult NannersGetF32BE(const uint8_t* payload, uint8_t payload_len, uint8_t* pos, float* out){

  return NANNERS_CODEC_OK;
}