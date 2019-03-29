/*
 * This file defines functions that can be called by either Arduino that is on either end of the data
 * stream setup (i.e. the 'first' and 'last' Arduinos) to carry out serial communication. These Arduinos
 * will only receive and send data from/to the one Arduino that they're connected to.
 * This Arduino only utilizes the AltSoftSerial software serial port to receive and send data. This frees
 * up the hardware Serial port to be used for debugging.
 */

#include <AltSoftSerial.h>
#include <RingBuf.h>
#include "Serial_FL.h"

//#define PACKET_SIZE 14

void sendData(AltSoftSerial &serialTwo, char packet[PACKET_SIZE+1]){
  serialTwo.write(packet, PACKET_SIZE+1);
}

void processData(RingBuf *buf) {
  while(!buf->isEmpty(buf)){
    char packet[PACKET_SIZE];
    buf->pull(buf, &packet);
    Serial.write(packet, PACKET_SIZE);//just output it for now
  }
}


void processIncomingByte(byte inByte, int &read_flag, RingBuf *buf){
  static char input[PACKET_SIZE];
  static uint32_t input_idx = 0;

  switch(inByte) {
    case '\n':
      if (input_idx != PACKET_SIZE-1) {
        input_idx = 0;
        read_flag = 0;
        break;
      }
      input[input_idx] = '\n';
      buf->add(buf, &input);
      processData(buf);//just putting this here for now, maybe we'll call it somewhere else idk
      input_idx = 0;
      read_flag = 0;
      break;
      
    case '\r':
      break;

    case '\t':
      break;
      
    default:
      if (input_idx < PACKET_SIZE-1) {
        input[input_idx++] = inByte;
        //Serial.write(input[input_idx-1]);
        break;
      }
  }
  
}

void readData(AltSoftSerial &serialTwo, int &read_flag, RingBuf *buf){
  while (serialTwo.available() > 0){
    char c = serialTwo.read();
    if (read_flag == 1){
      processIncomingByte(c, read_flag, buf);
      if (c == '\n'){
        break;
      }
    }
    if (c == '\t'){
      read_flag = 1;
    }
  }
}
