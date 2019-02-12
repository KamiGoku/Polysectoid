#include <Pattern.h>
#include <BasicLinearAlgebra.h>
#include <NeoSWSerial.h>
#include <math.h>
#include <RingBuf.h>

#define PACKET_SIZE 14

RingBuf *buf = RingBuf_new(PACKET_SIZE * sizeof(char), 10); // Buffer that holds 10 packets

uint32_t startTime = 0;
//int elapsedTime = 0;
int random_bool = 0;
int read_flag = 0;

NeoSWSerial serialOne(10, 11);

void setup() {
  Serial.begin(9600);
  while (!Serial);
  serialOne.begin(9600);

  /*cli();//stop interrupts

  //set timer0 interrupt at 2kHz
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

// This ISR will receive packets sent from
// another arduino and put them in the buffer
//ISR (TIMER0_COMPA_vect){
  // Read a packet if there is one
  /*if(Serial.available() >= 5){
    char packet[5];
    Serial.readBytes(packet,5);
    if(!buf->isFull){
      buf->add(buf, &packet);
    }
  }*/

   /*while (serialOne.available() > 0){
    //processIncomingByte(Serial.read());
    
    //processIncomingByte(serialOne.read());
    char c = serialOne.read();
    processIncomingByte(c);
    
    //processData();
  }*/

//}

/*void processIncomingPacket(char packet[5]) {
  //Serial.write(packet, 5);
  if (!buf->isFull(buf)){
    buf->add(buf, &packet);
  }
}*/

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
      input[input_idx] = '\n';
      buf->add(buf, &input);
      processData();//just putting this here for now, maybe we'll call it somewhere else idk
      input_idx = 0;
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

// In here we are just outputting packets from the buffer
void loop() {
  
  /*if(!buf->isEmpty){
    char packet[5];
    buf->pull(buf, &packet);
    Serial.write(packet, 5);
  }*/

  /*while (Serial.available() >= 5){
    char packet[5];
    Serial.readBytes(packet,5);
    //Serial.write(packet, 5);
    processIncomingPacket(packet);
    processData();
  }*/

  while (serialOne.available() > 0){
    char c = serialOne.read();
    processIncomingByte(c);
  }
  
  /*if (serialOne.available() > 0){
   //processIncomingByte(Serial.read());

    char c = serialOne.read();
    if (c == '\t') {
      while (serialOne.available() > 0) {
        char c = serialOne.read();
        processIncomingByte(c);
        if (c == '\n') {
          break;
        }
      }
      //processData();
    }
  }*/

}
