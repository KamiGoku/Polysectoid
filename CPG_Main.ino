#include <Oscillator.h>
#include <BasicLinearAlgebra.h>
#include <math.h> 
#include <Pattern.h>

#define scale 1000 //factor 10^3 doe millisecond

template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; } //To make the " Serial << output " syntax possible
const double dt=0.002;

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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  /*// put your main code here, to run repeatedly:
  double weight[5]={3.2,4.5,7.8,9,10};
  Oscillator testSeg(1,0,weight);
  //Serial << sizeof(double);
  for(int i=0;i<5;i++){
    Serial << (double) testSeg.getWeight(i) << '\n';
    delay(800);
  }*/
}
