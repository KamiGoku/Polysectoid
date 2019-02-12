#include <Pattern.h>
#include <BasicLinearAlgebra.h>
#include <NeoSWSerial.h>
#include <math.h>
#include <RingBuf.h>

#define NO_BOARD 1

#ifdef NO_BOARD
int outVal = 0;
int sub = 0;
int add_this = 1;
int my_idx = 0;
double my_array[] = {0.1, 0.11, 0.4, 0.2, 0.8, 0.88123, 0.1828939, 0.999, 0.76, 0.7719, 0.887, 0.65, 0.44, 0.5556, 0.21, 0.2, 0.11, 0.6, 0.33, 0.22, 0.88, 0.66, 0.55, 0.6613};
#endif

int sensorPin1 = A0;                              // One of the sensors
int sensorPin2 = A1;                              // The other sensor in the same segment   
//RingBuf *buf = RingBuf_new(5 * sizeof(char), 10); // Buffer that holds 10 packets

unsigned long prev_time = 0;
unsigned long curr_time;

/*typedef union {
    double f;
    struct {
      uint32_t mantissa : 23;
      uint32_t exponent : 8;
      uint32_t sign : 1;
    } parts;
  } float_cast;*/

NeoSWSerial serialOne(10, 11);

void setup() {
  pinMode(sensorPin1,INPUT);
  pinMode(sensorPin2,INPUT);
  Serial.begin(9600);
  while (!Serial);
  serialOne.begin(9600);

  /*cli(); //disable interrupts 
  
  //set timer2 interrupt at 1kHz
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

  sei(); //enable interrupts*/
}

// This ISR will gather sensor data
/*ISR(TIMER2_COMPA_vect){
  //might put analogReads in here later
}*/


// In here we are just filling up the buffer with packets
void loop() {
  //uint32_t startTime = micros();

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
  int adcVal = s_val1;
  /*int adcVal = s_val2;
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
  /*adcVal = my_array[my_idx]*add_this;
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

  //double val = 0.1234567233;
  
  char packet[15];
  dtostrf(my_array[my_idx], 13, 11, packet+1);
  my_idx = (my_idx+1)%24;
  packet[0] = '\t';
  packet[13] = '\n';

  /*float_cast d1 = {.f = 3.14159};
  Serial.println(sizeof(d1));
  Serial.println(d1.parts.sign, HEX);
  Serial.println(d1.parts.exponent, HEX);
  Serial.println(d1.parts.mantissa, HEX);
  while(1);*/

  
  
  serialOne.write(packet, 15);
  //Serial.write(packet, 14);
  //uint32_t elapsedTime = micros() - startTime;
  //Serial.println(elapsedTime);

  /*if(!buf->isFull(buf)){
    buf->add(buf,&packet);
  }*/
  
}
