#include <Pattern.h>
#include <BasicLinearAlgebra.h>
#include <NeoSWSerial.h>
#include <AltSoftSerial.h>
#include <math.h>
#include <RingBuf.h>

#define PACKET_SIZE 14

//NeoSWSerial serialOne(2,3); //RX, TX, will receive on pin2 RX
HardwareSerial &serialOne = Serial;
AltSoftSerial serialTwo; //pins 8,9, will send from pin9 TX

double my_array[10] = {0.555, 0.666, 0.777, 0.888, 0.999, 0.999, 0.888, 0.777, 0.36, 0.555};
uint32_t my_idx = 0;

int read_flag = 0;

RingBuf *buf = RingBuf_new(PACKET_SIZE * sizeof(char), 10); // Buffer that holds 10 packets

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  while (!Serial);
  //serialOne.begin(9600);
  serialTwo.begin(9600);

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

void processData() {
  while(!buf->isEmpty(buf)){
    char packet[PACKET_SIZE];
    buf->pull(buf, &packet);
    Serial.write(packet, PACKET_SIZE);//just output it for now
  }
}

void processIncomingByte(byte inByte){
  static char input[PACKET_SIZE];
  static uint32_t input_idx = 0;

  switch(inByte) {
    case '\n':
      if (input_idx != PACKET_SIZE-1){
        input_idx=0;
        read_flag=0;
        break;
      }
      input[input_idx] = '\n';
      buf->add(buf, &input);
      processData();//just putting this here for now, maybe we'll call it somewhere else idk
      input_idx = 0;
      read_flag=0;
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

void loop() {
  // put your main code here, to run repeatedly:

  char packet[15];
  dtostrf(my_array[my_idx], 13, 11, packet+1);
  my_idx = (my_idx+1)%10;
  packet[0] = '\t';
  packet[14] = '\n';

  serialTwo.write(packet, 15);

  //Serial.println(serialTwo.available());
  while (serialTwo.available() > 0){
    char c = serialTwo.read();
    if (read_flag == 1){
      processIncomingByte(c);
    }
    if (c == '\t'){
      read_flag = 1;
    }
  }


}
