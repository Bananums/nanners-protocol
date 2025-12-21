//
// // Created by Bananums: https://github.com/Bananums on 12/21/25.
//

#include <stdio.h>

#include <nanners/nanners.h>
#include <nanners/version.h>

int main(){

  printf("Running consumer test for nanners-protocol version %s\n", NANNERS_VERSION_STRING);


  uint8_t payload[8];
  uint8_t len = 0;

  const NannersPutResult result = NannersPutU8(payload, &len, (uint8_t)12);
  if (result != NANNERS_PUT_OK) {
    return 1;
  }

  printf("Consumer test succeeded.\n");
  return 0;
}