#include <Pattern.h>
#include <BasicLinearAlgebra.h>
#include <SoftwareSerial.h>
#include <math.h>
#include <RingBuf.h>


int sensorPin1 = A0;                              // One of the sensors
int sensorPin2 = A1;                              // The other sensor in the same segment   
RingBuf *buf = RingBuf_new(5 * sizeof(char), 10); // Buffer that holds 10 packets

void setup() {
  pinMode(sensorPin1,INPUT);
  pinMode(sensorPin2,INPUT);
  Serial.begin(9600);

  /*cli(); //disable interrupts 
  
  // Setup Timer 1 to trigger every 30 Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 520;// = (16*10^6) / (30*1024) - 1 (must be <65536)  520
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) || (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei(); //enable interrupts*/
}

// This ISR will send the packets stored in the
// buffer to another arduino via the TX port
/*ISR(TIMER1_COMPA_vect){
  if(!buf->isEmpty){
    char packet[5];
    buf->pull(buf,&packet);
    Serial.write(packet, 5);
  }
}*/


// In here we are just filling up the buffer with packets
void loop() {

  int s_val1 = analogRead(sensorPin1);
  int s_val2 = analogRead(sensorPin2);

  // Calibrating left sensor to 0-500
  if (s_val1 <= 0) s_val1 = 0;
  else if (s_val1 > 605) s_val1 = 605;
  s_val1 = ((s_val1 - 0)*500)/605;
  s_val1 = 500 - s_val1;//flip


  // Calibrating right sensor to 0-500
  if (s_val2 < 30) s_val2 = 30;
  else if (s_val2 > 450) s_val2 = 450;
  s_val2 = ((s_val2 - 30)*500)/420;

  // Average left and right vals
  int adcVal = (s_val1 + s_val2) / 2;

  // Converting data into a packet and storing it in buffer
  char packet [5];
  packet[4] = '\n';
  packet[3] = (adcVal % 10) + 0x30;
  adcVal = adcVal/10;
  packet[2] = (adcVal % 10) + 0x30;
  adcVal = adcVal/10;
  packet[1] = (adcVal % 10) + 0x30;
  adcVal = adcVal/10;
  packet[0] = (adcVal % 10) + 0x30;

  Serial.write(packet, 5);

  /*if(!buf->isFull(buf)){
    buf->add(buf,&packet);
  }*/
  
}
