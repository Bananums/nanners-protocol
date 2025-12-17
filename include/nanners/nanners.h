//
//Created by Bananums: https://github.com/Bananums on 11/5/25.
//

#ifndef NANNERS_PROTOCOL_INCLUDE_NANNERS_NANNERS_H
#define NANNERS_PROTOCOL_INCLUDE_NANNERS_NANNERS_H

#include <stdint.h>
#include <stddef.h>
#include "nanners/frame.h"
#include "nanners/structs.h"

#ifdef __cplusplus
extern "C" {
#endif

void NannersInit(NannersFrame* frame);
void NannersReset(NannersFrame* frame);
NannersResult NannersProcessByte(NannersFrame* frame, uint8_t byte, NannersStats *stats);
int32_t NannersSerializeFrame(uint16_t frame_id, uint8_t seq,
                              const uint8_t* payload, uint8_t length,
                              uint8_t* out_wire, size_t out_wire_len);
uint16_t ComputeFrameCrc(const NannersFrame* frame);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif //NANNERS_PROTOCOL_INCLUDE_NANNERS_NANNERS_H
