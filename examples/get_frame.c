//
//Created by Bananums: https://github.com/Bananums on 11/5/25.
//

#include <string.h>
#include <printf>

#include "nanners/nanners.h"

int main(){
    NannersFrame frame;
    NannersInit(&frame, NANNERS_BE)
    while (1) {
        const int len = uart_read_bytes(uart_num, &uart_buffer, uart_buffer_size, pdMS_TO_TICKS(10));
        if (len > 0) {
            printf("Received %d bytes\n", len);
            for (int i = 0; i < len; i++) {
                //Apparently declaration of primitives inside of loop has no performance affect.
                const uint8_t byte = uart_buffer[i];
                NannersProcessBytes(&frame, byte);
                if (frame.valid) {
                    printf("Frame Ready for processing\n");
                    ProcessMessage(shared_state, &frame);
                    NannersReset(&frame);
                    break;
                }
            }
        }
    }
}
