//
// Created by snackers on 11/10/25.
//

#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "nanners/nanners.h"  // your API

int main(){
  setvbuf(stdout, NULL, _IONBF, 0);


  NannersFrame in;
  NannersInit(&in);
  in.frame_id = 0x01;
  in.seq = 0x02;
  in.length = 0x03;
  in.payload[0] = 0xAA;
  in.payload[1] = 0xBB;
  in.payload[2] = 0xCC;
  in.crc = ComputeFrameCrc(&in);

  printf("Expected crc: %hu\n", in.crc);

  return 0;
}