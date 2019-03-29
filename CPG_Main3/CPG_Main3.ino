#include "cpg.cpp"
#include <PWM.h>
#include <FastPID.h>

float Kp=0.1, Ki=0, Kd=0.5, Hz=1000;
int output_bits = 10;
bool output_signed = true;

FastPID myPID(Kp, Ki, Kd, Hz, output_bits, output_signed);
int inputPin = A0;                             
int pwmPin = 3;

Actuator actuators[NUM*3];
float phases[NUM*3];

Brain b;

void setup() {
  Serial.begin(9600);
  pinMode(inputPin, INPUT);
  pinMode(pwmPin, OUTPUT);

  //printTables();

  // Create 15 Oscillators
  for(int i = 0; i < NUM*3; i++){
    actuators[i].index = i;
  }

}
void loop() {
  for(int i = 0; i < NUM*3; i++){
    float rel_phases [SIZE];
    if(i < 5){
      for(int j = 0; j < 5; j++){
        rel_phases[j] = phases[j];
      }
      rel_phases[6] = phases[i+5];
      rel_phases[7] = phases[i+10];
    }
    else if(i <= 10){
      for(int j = 0; j < 5; j++){
        rel_phases[j] = phases[j+5];
      }
      rel_phases[6] = phases[i+5];
      rel_phases[7] = phases[i-5];
    }
    else{
      for(int j = 0; j < 5; j++){
        rel_phases[j] = phases[j+10];
      }
      rel_phases[6] = phases[i-10];
      rel_phases[7] = phases[i-5];
    }
    
    float curr_phase = actuators[i].update_phase(rel_phases);
    phases[i] = curr_phase;
    
    if(i == 0){
      int expected = analogRead(inputPin);
      expected = 500;
      int actual = curr_phase*1024;
      int output = myPID.step(expected, actual);
      printPID(expected, actual, output);
      //Serial.print(actual);
      //Serial.println(output);
      pwmWrite(pwmPin, output);
    }
    
    delay(1);

    //Serial.print(10*curr_phase);
    //i != 14 ? Serial.print(" ") : Serial.println();
  } 
}














///////////////////////////////// HELPER FUNCTIONS /////////////////////////////////
void printPID(int expected, int actual, int output){
    Serial.print(F("Expected: "));
    Serial.print(expected);
    Serial.print(F("     Actual: "));
    Serial.println(actual);
    Serial.print(F("Output: "));
    Serial.println(output);   
}



void printTables(){
  Serial.println(F("WEIGHT TABLE:"));
  for(int i = 0; i < 15; i++){
    for(int j = 0; j < SIZE; j++){
      Serial.print(((int)b.weights_bias[i][j])/10000.0,4);
      Serial.print(F(" "));
    }
    Serial.println();
  }
  Serial.println(F(" "));

  Serial.println(F("BIAS TABLE:"));
  for(int i = 0; i < 15; i++){
    for(int j = 0; j < SIZE; j++){
      double intpart,fracpart;
      fracpart = modf(b.weights_bias[i][j], &intpart);
      Serial.print(fracpart,4);
      Serial.print(F(" "));
    }
    Serial.println();
  }
  Serial.println(F(" ")); 
}

