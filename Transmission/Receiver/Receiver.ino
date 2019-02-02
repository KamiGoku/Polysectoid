#include <Pattern.h>
#include <BasicLinearAlgebra.h>
#include <SoftwareSerial.h>
#include <math.h>
#include <RingBuf.h>


RingBuf *buf = RingBuf_new(5 * sizeof(char), 10); // Buffer that holds 10 packets

void setup() {
  Serial.begin(9600);

  /*cli(); //disable interrupts 
  
  //Setup Timer 0 to trigger every 66 Hz
  TCCR0A = 0;// set entire TCCR0A register to 0
  TCCR0B = 0;// same for TCCR0B
  TCNT0  = 0;//initialize counter value to 0
  // set compare match register for 2khz increments
  OCR0A = 236;// = (16*10^6) / (66*1024) - 1 (must be <256)     236
  // turn on CTC mode
  TCCR0A |= (1 << WGM01);
  // Set CS01 and CS00 bits for 1024 prescaler
  TCCR0B |= (1 << CS02) | (1 << CS00);   
  // enable timer compare interrupt
  TIMSK0 |= (1 << OCIE0A);

  sei(); //enable interrupts*/

}

// This ISR will receive packets sent from
// another arduino and put them in the buffer
/*ISR (TIMER0_COMPA_vect){
  // Read a packet if there is one
  if(Serial.available() >= 5){
    char packet[5];
    Serial.readBytes(packet,5);
    if(!buf->isFull){
      buf->add(buf, &packet);
    }
  }

}*/

/*void processIncomingPacket(char packet[5]) {
  //Serial.write(packet, 5);
  if (!buf->isFull(buf)){
    buf->add(buf, &packet);
  }
}*/

void processData() {
  while(!buf->isEmpty(buf)){
    char packet[5];
    buf->pull(buf, &packet);
    Serial.write(packet, 5);//just output it for now
  }
}

void processIncomingByte(byte inByte){
  static char input[5];
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
      
    default:
      if (input_idx < 4) {
        input[input_idx++] = inByte;
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

  while (Serial.available() > 0){
    processIncomingByte(Serial.read());
    //processData();
  }

}
