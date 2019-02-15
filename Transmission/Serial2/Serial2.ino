#include <Pattern.h>
#include <BasicLinearAlgebra.h>
#include <NeoSWSerial.h>
#include <AltSoftSerial.h>
#include <math.h>
#include <RingBuf.h>

#define PACKET_SIZE 14

NeoSWSerial serialOne(2, 3); //RX, TX
AltSoftSerial serialTwo; //pins 8,9

RingBuf *buf = RingBuf_new(PACKET_SIZE * sizeof(char), 10); // Buffer that holds 10 packets
RingBuf *buf2 = RingBuf_new(PACKET_SIZE * sizeof(char), 10); // 2nd buffer for third arduino

int read_flag = 0;
int read_flag2 = 0;
int muh_bool = 0;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  while (!Serial);
  serialTwo.begin(9600);
  serialOne.begin(9600);

  /*cli(); //disable interrupts 
  
  //set timer2 interrupt at 1kHz
  //Timer2 interrupt is for transmitting 
  TCCR2A = 0;// set entire TCCR2A register to 0
  TCCR2B = 0;// same for TCCR2B
  TCNT2  = 0;//initialize counter value to 0
  // set compare match register for 1khz increments
  OCR2A = 249;// = (16*10^6) / (1000*64) - 1 (must be <256)
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  // Set CS22 bit for 64 prescaler
  TCCR2B |= (1 << CS22); 
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);

  //set timer0 interrupt at 2kHz
  //Timer0 interrupt is for receiving
  TCCR0A = 0;// set entire TCCR0A register to 0
  TCCR0B = 0;// same for TCCR0B
  TCNT0  = 0;//initialize counter value to 0
  // set compare match register for 2khz increments
  OCR0A = 124;// = (16*10^6) / (2000*64) - 1 (must be <256)
  // turn on CTC mode
  TCCR0A |= (1 << WGM01);
  // Set CS01 and CS00 bits for 64 prescaler
  TCCR0B |= (1 << CS01) | (1 << CS00);   
  // enable timer compare interrupt
  TIMSK0 |= (1 << OCIE0A);

  sei(); //enable interrupts*/

}

void processData(int which_serial) {
  if (which_serial == 2) {
    while(!buf->isEmpty(buf)){
      char packet[15];
      char *packet_ptr = packet+1;
      buf->pull(buf, packet_ptr);
      packet[0] = '\t';
      serialTwo.write(packet, 15);
      //Serial.write(packet, PACKET_SIZE+1);
    }
  } else if (which_serial == 1) {
    while(!buf2->isEmpty(buf2)){
      char packet[15];
      char *packet_ptr = packet+1;
      buf2->pull(buf2, packet_ptr);
      packet[0] = '\t';
      serialOne.write(packet, 15);
      //Serial.write(packet, 15);
    }
    return;
  }
  return;
}

void processIncomingByte(byte inByte, int which_buf){
  static char input[PACKET_SIZE];
  static char input2[PACKET_SIZE];
  static uint32_t input_idx = 0;
  static uint32_t input_idx2 = 0;

  switch(inByte) {
    case '\n':
      if (which_buf == 1) {
        if (input_idx != PACKET_SIZE-1) {
          input_idx = 0;
          read_flag = 0;
          break;
        }
        input[input_idx] = '\n';
        //Serial.write(input, PACKET_SIZE);
        buf->add(buf, &input);
        input_idx = 0;
        read_flag = 0;
        processData(2);//just putting this here for now, maybe we'll call it somewhere else idk
        break;
      } else {
        if (input_idx2 != PACKET_SIZE-1) {
          input_idx2 = 0;
          read_flag2 = 0;
          break;
        }
        input2[input_idx2] = '\n';
        buf2->add(buf2, &input2);
        input_idx2 = 0;
        read_flag2 = 0;
        processData(1);//just putting this here for now, maybe we'll call it somewhere else idk
        break;
      }
      break;
      
    case '\r':
      break;

    case '\t':
      break;
      
    default:
    if (which_buf == 1) {
      if (input_idx < PACKET_SIZE-1) {
        input[input_idx++] = inByte;
        //Serial.write(input[input_idx-1]);
        break;
      }
    } else {
      if (input_idx2 < PACKET_SIZE-1) {
        input2[input_idx2++] = inByte;
        //Serial.write(input[input_idx-1]);
        break;
      }
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  //Serial.println(serialOne.available());
  while (serialOne.available() > 0){
    char c = serialOne.read();
    if (read_flag == 1){
      processIncomingByte(c, 1);
    }
    if (c == '\t'){
      read_flag = 1;
    }
  }

  /*//some 1-on-1 testing
  char my_packet[15];
  my_packet[0] = '\t';
  if (muh_bool == 0) {
    dtostrf(0.222, 13, 11, my_packet+1);
    muh_bool = 1;
  } else {
    dtostrf(0.333, 13, 11, my_packet+1);
    muh_bool = 0;
  }
  my_packet[14] = '\n';
  serialOne.write(my_packet, 15);*/

  /*char packet[PACKET_SIZE];
  if (!buf->isEmpty(buf)) {
    buf->pull(buf, &packet);
    serialTwo.write(packet, PACKET_SIZE);//just output it for now
  }*/

  while (serialTwo.available() > 0){
    char c = serialTwo.read();
    if (read_flag2 == 1){
      processIncomingByte(c, 2);
    }
    if (c == '\t'){
      read_flag2 = 1;
    }
  }

  /*char packet2[PACKET_SIZE];
  //if (!buf2->isEmpty(buf2)) {
    //buf2->pull(buf2, &packet2);
//    /serialOne.write(packet2, PACKET_SIZE);
  //}*/

}
