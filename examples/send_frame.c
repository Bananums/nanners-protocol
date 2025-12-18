//
// Created by Bananums: https://github.com/Bananums on 12/17/25.
//

#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "nanners/nanners.h"


int32_t WriteBytesSim(const uint8_t *byte_buffer, const size_t buffer_len) {
    for (int i = 0; i < buffer_len; ++i) {
        printf("Writing byte %i: %02X \n", i, byte_buffer[i]);
    }

    return (int32_t)(buffer_len);
}

// Example using a payload of 5 bytes.
typedef struct {
    uint8_t action; // 1 byte
    float steering; // 4 byte
    bool heartbeat; // 4 byte
} CommandPayload;

// Everything above int main() is just to set up the example
int main(){
    uint8_t wire[16]; // 16 bytes is maximum frame size

    CommandPayload command;
    command.action = 1;
    command.steering = 12.23f;
    command.heartbeat = false;
    const uint8_t seq = 12;
    uint8_t payload[8]; // 8 bytes is maximum payload size
    uint8_t payload_len = 0;

    NannersPutU8(payload, &payload_len, command.action);
    NannersPutF32BE(payload, &payload_len, command.steering);
    NannersPutBoolean(payload, &payload_len, command.heartbeat);

    const int32_t n = NannersSerializeFrame(9, seq, payload, payload_len, wire, sizeof(wire));

    if (n < 0) {
        return n;
    }

    const int32_t len = WriteBytesSim(wire, (size_t)(n));
    printf("Wrote %i bytes\n", (int)len);

    return 0;
}