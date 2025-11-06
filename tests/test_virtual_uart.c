//
// Created by snackers on 11/5/25.
//
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "nanners/nanners.h"  // your API


int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0); // Display prints, even if an assert fails
    // Build a frame in-memory and feed bytes to your parser (virtual UART).
    const uint16_t frame_id = 0x0123; // 291
    const uint8_t payload[] = {0x10, 0x20, 0x30};
    uint8_t wire[64];
    size_t n = 0;

    NannersFrame frame;
    NannersInit(&frame);

    wire[n++] = (uint8_t)NANNERS_START_OF_FRAME;
    wire[n++] = (uint8_t)(frame_id >> 8);
    wire[n++] = (uint8_t)(frame_id & 0xFF);
    wire[n++] = (uint8_t)sizeof(payload);
    memcpy(&wire[n], payload, sizeof(payload)); n += sizeof(payload);

    // 13226
    wire[n++] = 0x33;
    wire[n++] = 0xaa;

    wire[n++] = (uint8_t)NANNERS_END_OF_FRAME;

    // Feed byte-by-byte
    for (size_t i = 0; i < n; ++i) {
        NannersProcessBytes(&frame, wire[i]);
    }

    // Check result
    assert(frame.valid == true);

    // Print result
    printf("Expected frame_id: %u, got: %u\n", frame_id, frame.frame_id);
    printf("Expected length: %zu, got: %u\n",
           sizeof(payload), frame.length);
    printf("Payload bytes:\n");
    for (size_t i = 0; i < frame.length; ++i)
        printf("  [%02zu] expected: %02X  got: %02X\n",
               i, payload[i], frame.payload[i]);
    printf("\n");

    assert(frame.frame_id == frame_id);
    assert(frame.length == sizeof(payload));
    assert(memcmp(frame.payload, payload, sizeof(payload)) == 0);

    return 0;
}

//#if NANNERS_ENABLE_CRC
  //uint8_t crc_src[3 + sizeof(payload)];
  //crc_src[0] = (uint8_t)(frame_id >> 8);
  //crc_src[1] = (uint8_t)(frame_id & 0xFF);
  //crc_src[2] = (uint8_t)sizeof(payload);
  //memcpy(&crc_src[3], payload, sizeof(payload));
  //const uint16_t crc = nanners_crc16(crc_src, sizeof(crc_src));
  //wire[n++] = (uint8_t)(crc >> 8);
  //wire[n++] = (uint8_t)(crc & 0xFF);
//#else
  //wire[n++] = 0u;
  //wire[n++] = 0u;
//#endif
