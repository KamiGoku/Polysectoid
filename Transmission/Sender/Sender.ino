#include <Pattern.h>
#include <BasicLinearAlgebra.h>
#include <SoftwareSerial.h>
#include <math.h>
#include <RingBuf.h>

#define NO_BOARD 1

#ifdef NO_BOARD
int outVal = 0;
int sub = 0;
int add_this = 1;
int my_idx = 0;
double my_array[] = {0.1, 1.1, 1.2, 2.2, 3.5, 5.5, 8.5, 13.55, 21.4, 34.4, 55.5, 89.6, 144.2, 143.2, 131.2, 104.4, 88.5, 77.5, 53.6, 32.8, 17.8, 13.1, 7.1, 3.1};
#endif

int sensorPin1 = A0;                              // One of the sensors
int sensorPin2 = A1;                              // The other sensor in the same segment   
//RingBuf *buf = RingBuf_new(5 * sizeof(char), 10); // Buffer that holds 10 packets

unsigned long prev_time = 0;
unsigned long curr_time;

SoftwareSerial serialOne(10, 11);

void setup() {
  pinMode(sensorPin1,INPUT);
  pinMode(sensorPin2,INPUT);
  Serial.begin(9600);
  while (!Serial);
  serialOne.begin(9600);

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
  uint32_t startTime = micros();

  int s_val1 = analogRead(sensorPin1);
  //Serial.println(s_val1);
  int s_val2 = analogRead(sensorPin2);
  //Serial.println(s_val2);

  // Calibrating left sensor to 0-500
  /*if (s_val1 <= 800) s_val1 = 800;
  else if (s_val1 > 1030) s_val1 = 1030;
  s_val1 = ((s_val1 - 800)*500)/230;*/
  //s_val1 = 1030 - s_val1;//flip


  // Calibrating right sensor to 0-500
  /*if (s_val2 < 920) s_val2 = 920;
  else if (s_val2 > 1030) s_val2 = 1030;
  s_val2 = ((s_val2 - 920)*500)/110;*/

  // Average left and right vals
  //int adcVal = (s_val1 + s_val2) / 2;
  int adcVal = s_val2;
  //Serial.println(adcVal);

  #ifdef NO_BOARD
  adcVal = outVal;
  /*if (outVal == 250) { 
    sub = 1;
    add_this = 1;
  }
  else if (outVal == 0) {
    sub = 0;
    add_this = 1;
  }
  if (sub) {
    outVal = outVal - add_this;
    add_this++;
  }
  else {
    outVal = outVal + add_this;
    add_this++;
  }*/
  adcVal = my_array[my_idx]*add_this;
  my_idx = (my_idx+1)%24;
  if (my_idx == 0) {
    add_this = (add_this+1)%4;
  }
  #endif

  // Converting data into a packet and storing it in buffer
  /*char packet [5];
  packet[4] = '\n';
  packet[3] = (adcVal % 10) + 0x30;
  adcVal = adcVal/10;
  packet[2] = (adcVal % 10) + 0x30;
  adcVal = adcVal/10;
  packet[1] = (adcVal % 10) + 0x30;
  adcVal = adcVal/10;
  packet[0] = (adcVal % 10) + 0x30;*/
  char packet[7];
  dtostrf(my_array[my_idx], 6, 2, packet);
  my_idx = (my_idx+1)%24;
  packet[6] = '\n';

  serialOne.write(packet, 7);
  Serial.write(packet, 7);
  uint32_t elapsedTime = micros() - startTime;
  //Serial.println(elapsedTime);

  /*if(!buf->isFull(buf)){
    buf->add(buf,&packet);
  }*/
  
}
