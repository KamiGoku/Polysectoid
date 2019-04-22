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

extern RingBuf *brain_buf;
extern RingBuf *seg_buf;

void sendData(AltSoftSerial &serialTwo, char *packet, int packetsize){
  char packet_[125];
  memcpy(packet_+1, packet, packetsize);
  packet_[0] = '\t';
  //Serial.write(packet,125);
  serialTwo.write(packet_, packetsize);
}

void processData(RingBuf *buf) {
  while(!buf->isEmpty(buf)){
    char packet[PACKET_SIZE];
    buf->pull(buf, &packet);
    Serial.write(packet, PACKET_SIZE);//just output it for now
  }
}

//change this so input is of size 124
//change it to take buf_size as an argument, or make buf_size a global
void processIncomingByte(byte inByte, int &read_flag, int buf_size){
  static char s_input[14];
  static char input[124];
  static uint32_t input_idx = 0;

  switch(inByte) {
    case '\n':
      if (input_idx != buf_size-1) {
        input_idx = 0;
        read_flag = 0;
        break;
      }
      input[input_idx] = '\n';
      if (buf_size == 124) {
        //Serial.write(input,124);
        brain_buf->add(brain_buf, &input);
        /*char *arr = (char *) malloc(124*sizeof(char));
        brain_buf->pull(brain_buf, arr);
        Serial.write(arr,124);
        while(1);*/
      } else {
        memcpy(s_input, input, 14);
        seg_buf->add(seg_buf, &s_input);
      }
      //processData(buf);//just putting this here for now, maybe we'll call it somewhere else idk
      input_idx = 0;
      read_flag = 0;
      break;
      
    case '\r':
      break;

    case '\t':
      break;
      
    default:
      if (input_idx < buf_size-1) {
        input[input_idx++] = inByte;
        //Serial.write(input[input_idx-1]);
        break;
      }
  }
  
}

void readData(Stream &serialTwo, int &read_flag){
  static int buf_size = 0;
  //Serial.println(serialTwo.available());
  while (serialTwo.available() > 0){
    char c = serialTwo.read();
    if (read_flag == 2){
      if (c == 's') {
        buf_size = 14;
        read_flag = 1;
      }
      else if (c == 'b') {
        buf_size = 124;
        read_flag = 1;
      }
      else {
        read_flag = 0;
      }
    }
    if (read_flag == 1){
      processIncomingByte(c, read_flag, buf_size);
      if (c == '\n'){
        break;
      }
    }
    if (c == '\t'){
      read_flag = 2;
    }
  }
}
