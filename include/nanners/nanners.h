//
//Created by Bananums: https://github.com/Bananums on 11/5/25.
//

#ifndef NANNERS_PROTOCOL_INCLUDE_NANNERS_NANNERS_H
#define NANNERS_PROTOCOL_INCLUDE_NANNERS_NANNERS_H

#include <stdint.h>
#include "frame.h"

#ifdef __cplusplus
extern "C" {
#endif

void NannersInit(NannersFrame* frame);
void NannersReset(NannersFrame* frame);
void NannersProcessByte(NannersFrame* frame, uint8_t byte);
uint16_t ComputeFrameCrc (const NannersFrame* frame);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif //NANNERS_PROTOCOL_INCLUDE_NANNERS_NANNERS_H
