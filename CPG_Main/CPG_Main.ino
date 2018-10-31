#include <Oscillator.h>
#include <BasicLinearAlgebra.h>
#include <math.h>
#include <Pattern.h>
#include <RingBuf.h>


#define scale 1000 //factor 10^3 doe millisecond
#define FS 15 //size of filter buffer

template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; } //To make the " Serial << output " syntax possible
const double dt=0.002;
uint16_t filter_buffer[FS] = {0};
int filter_idx = 0;
bool isFull = false;
RingBuf *buf1 = RingBuf_new(sizeof(double), 10);
RingBuf *buf2 = RingBuf_new(sizeof(double), 10);
double testarr[10] = {1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0};
int buf_idx = 0;
unsigned long lastRead = 0;
unsigned long interval = 250;
double phaseUpdate(double phase, double trinFreq, double* inPhase, double* inWeight, double* inBias){//current phase, intrinsic frequency, coupled phases, coupled weight, coupled bias
  if(sizeof(inPhase)!=sizeof(inWeight)){
    return -1;//phase 0<=x<360
  }
  int N = sizeof(inPhase);
  double sum = 0;
  
  for(int i = 0; i < N; i++){
    sum += inWeight[i]*sin(inPhase[i] - phase - inBias[i]);//calculate the total weight phase
  }
  double temp = 2*pi*trinFreq + sum;
  temp = fmod(temp, 2*pi);
  return temp;
}

int sensorPin = A0;    // select the input pin for the potentiometer
int sensorPin2 = A1;
int sensorValue = 0;  // variable to store the value coming from the sensor

void setup() {
  // put your setup code here, to run once:

  //cli(); //disable interrupts 
  
  //first, set up interrupts
  //set timer0 interrupt at 2kHz
  /*TCCR0A = 0;// set entire TCCR0A register to 0
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

  //set timer1 interrupt at 4kHz
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
  TIMSK1 |= (1 << OCIE1A);*/

  //sei(); //enable interrupts
  
  Serial.begin(9600);
  pinMode(sensorPin, INPUT);
}

ISR(TIMER0_COMPA_vect){//timer0 interrupt ~66Hz
  if (!buf1->isEmpty(buf1)) {
    //there is one double in the buffer
    static double d;

    if(!buf2->isFull(buf2)){
          buf1->pull(buf1, &d);
          buf2->add(buf2, &d);
    }

  }

    /*if (!buf2->isEmpty(buf2)) {
      static double dd;
      buf2->pull(buf2, &dd);
      Serial << dd << '\n';
    }*/
}

ISR(TIMER1_COMPA_vect){//timer1 interrupt ~30Hz
  //This writes to buf1
  if (!buf1->isFull(buf1)) {
    buf1->add(buf1, &testarr[buf_idx]);
    buf_idx = (buf_idx + 1) % 10;
  }
}

void loop() {
  //Serial << analogRead(sensorPin2) << '\n';
  //delay(10);
  uint32_t s_val = analogRead(sensorPin);
  uint32_t s_val2 = analogRead(sensorPin2);
  //filter_buffer->add(filter_buffer, &s_val);
  
  //this is left
  if (s_val <= 0) s_val = 0;
  else if (s_val > 605) s_val = 605;
  s_val = ((s_val - 0)*500)/605;
  s_val = 500 - s_val;
  
  //this is right
  if (s_val2 < 30) s_val2 = 30;
  else if (s_val2 > 450) s_val2 = 450;
  s_val2 = ((s_val2 - 30)*500)/420;
  
  filter_buffer[filter_idx] = (s_val + s_val2) / 2;
  //filter_buffer[filter_idx] = s_val;

  filter_idx = (filter_idx + 1) % FS;
  if (filter_idx == 0) isFull = true;
  double avg;
  double sum = 0;
  int current_idx;
  if (isFull) current_idx = FS;
  else current_idx = filter_idx;
  for (int i = 0; i < current_idx; i++) {
    sum += (double) filter_buffer[i];
  }
  avg = sum/((double) current_idx);
  Serial << ((uint32_t) avg) << '\n';
  delay(10);

  //Serial << micros() << '\n';
  //unsigned long us = micros();
  /*Serial << us << '\n';
  if (us - lastRead >= interval) {
    lastRead += interval;
    Serial << lastRead << '\n';
    //Serial.println(micros());
    while (!buf2->isEmpty(buf2)) {
      static double d;
      buf2->pull(buf2, &d);
      //Serial << d << '\n';
    }
  }*/
  //Serial << 1;
/*
      while (!buf2->isEmpty(buf2)) {
        static double d;
        buf2->pull(buf2, &d);
        Serial << d << '\n';
      }
*/
  //delay(1000000);

  /*long start = micros();
  long end = 0;
  do {
    end = micros();
  } while (start + 250 >= end);*/
  
  /*// put your main code here, to run repeatedly:
  double weight[5]={3.2,4.5,7.8,9,10};
  Oscillator testSeg(1,0,0,weight);
  //Serial << sizeof(double);
  for(int i=0;i<5;i++){
    Serial << (double) testSeg.getWeight(i) << '\n';
    delay(800);
  }*/
}
