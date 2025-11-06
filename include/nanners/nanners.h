//
//Created by Bananums: https://github.com/Bananums on 11/5/25.
//

#ifndef NANNERS_PROTOCOL_SRC_NANNERS_H
#define NANNERS_PROTOCOL_SRC_NANNERS_H

#include <stdint.h>
#include <stdbool.h>
#include "frame.h"

#ifdef __cplusplus
extern "C" {
#endif

void NannersInit(NannersFrame* frame);
void NannersReset(NannersFrame* frame);
void NannersProcessBytes(NannersFrame* frame, uint8_t byte);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif //NANNERS_PROTOCOL_SRC_NANNERS_H
