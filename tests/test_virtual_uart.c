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
    printf("Starting NANNERS TEST: virtual uart");
    // Build a frame in-memory and feed bytes to your parser (virtual UART).
    const uint16_t frame_id = 0x0123; // 291
    const uint8_t seq = 0x67;
    const uint8_t len = 0x03;
    const uint8_t payload[8] = {0x10, 0x20, 0x30, 0x00, 0xF0, 0xAB, 0x00, 0x00};

    uint8_t wire[64];
    size_t n = 0;

    NannersFrame in;
    NannersInit(&in);
    in.frame_id = frame_id;
    in.seq = seq;
    in.length = len;
    in.payload[0] = payload[0];
    in.payload[1] = payload[1];
    in.payload[2] = payload[2];
    const uint16_t crc = ComputeFrameCrc(&in);

    NannersFrame frame;
    NannersInit(&frame);

    wire[n++] = (uint8_t)NANNERS_START_OF_FRAME;
    wire[n++] = (uint8_t)(frame_id >> 8);
    wire[n++] = (uint8_t)(frame_id & 0xFF);
    wire[n++] = (uint8_t)(seq);
    wire[n++] = (uint8_t)(len);
    memcpy(&wire[n], payload, len);
    n += len;
    wire[n++] = (uint8_t)(crc >> 8);
    wire[n++] = (uint8_t)(crc & 0xFF);
    wire[n++] = (uint8_t)NANNERS_END_OF_FRAME;

    // Feed byte-by-byte
    for (size_t i = 0; i < n; ++i) {
        NannersProcessByte(&frame, wire[i]);
    }

    // Check result
    assert(frame.valid == true);

    // Print result
    printf("Expected frame_id: %u, got: %u\n", frame_id, frame.frame_id);
    printf("Expected sequence: %hhu, got: %u\n", seq, frame.seq);
    printf("Expected length: %hhu, got: %u\n", len, frame.length);
    printf("Payload bytes:\n");
    for (size_t i = 0; i < frame.length; ++i) {
        printf("  [%02zu] expected: %02X  got: %02X\n", i, payload[i], frame.payload[i]);
    }
    printf("Expected CRC: %d, got: %u\n", crc, frame.crc);
    printf("\n");

    assert(frame.frame_id == frame_id);
    assert(frame.seq == seq);
    assert(frame.length == len);
    assert(memcmp(frame.payload, payload, len) == 0);
    assert(frame.crc == crc);
    return 0;
}
