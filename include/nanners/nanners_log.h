//
// Created by Bananums: https://github.com/Bananums on 12/16/25.
//

#ifndef NANNERS_PROTOCOL_INCLUDE_NANNERS_LOG_H
#define NANNERS_PROTOCOL_INCLUDE_NANNERS_LOG_H

#include <stdio.h>

#ifdef NANNERS_DEBUG
#define NANNERS_LOG(...) \
do { printf(__VA_ARGS__); } while (0)
#else
#define NANNERS_LOG(...) \
do { } while (0)
#endif

#endif //NANNERS_PROTOCOL_INCLUDE_NANNERS_LOG_H
