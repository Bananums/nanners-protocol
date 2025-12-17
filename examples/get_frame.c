//
//Created by Bananums: https://github.com/Bananums on 11/5/25.
//

#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "nanners/frame.h"
#include "nanners/nanners.h"

static volatile sig_atomic_t global_run = 1;

void HandleSignal(const int signal_catch) {
    (void) signal_catch;
    printf("Signal caught. Stopping\n");
    global_run = 0;
};

bool RegisterSignals() {
    struct sigaction signal_action;
    memset(&signal_action, 0, sizeof(signal_action));
    signal_action.sa_handler = HandleSignal;
    signal_action.sa_flags = 0; // allow EINTR
    sigemptyset(&signal_action.sa_mask);

    if (sigaction(SIGINT, &signal_action, NULL) == -1) {
        return false;
    }

    if (sigaction(SIGTERM, &signal_action, NULL) == -1) {
        return false;
    }

    return true;
};

int32_t ReadBytesSim(uint8_t *byte_buffer, size_t buffer_len) {
    //Example payload
    const uint16_t frame_id = 0x0123;
    const uint8_t seq = 0x67;
    const uint8_t len = 0x03;
    const uint8_t payload[8] = {0x10, 0x20, 0x30, 0x00, 0xF0, 0xAB, 0x00, 0x00};
    const uint16_t crc = 9285;

    //Filling simulated wire
    uint8_t wire[64];
    size_t n = 0;
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

    memcpy(byte_buffer, &wire, n);
    return (int32_t)(n);
}

// Everything above int main() is just to set up the example
int main(){
    if (!RegisterSignals()) {
        printf("Failed to register signals\n");
        return 1;
    }

    uint8_t read_buffer[64];
    size_t buffer_length = sizeof(read_buffer);

    NannersFrame frame;
    NannersInit(&frame);

    while(global_run) {
        const int32_t len = ReadBytesSim(read_buffer, buffer_length);
        printf("Received %i bytes\n", len);
        if (len <= 0) {
            usleep(1000000);
            continue;
        }

        for (int i = 0; i < len; i++) {
            const uint8_t byte = read_buffer[i];
            const NannersResult result = NannersProcessByte(&frame, byte, NULL);
            if (result == NANNERS_FRAME_READY) {
                printf("Frame Ready for processing\n");
                //Process message
                NannersReset(&frame);
                break;
            }
        }
        usleep(1000000);
    }

    return 0;
}
