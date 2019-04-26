#include "cpg.cpp"
#include <PWM.h>
#include <FastPID.h>
#include <Servo.h>
#include <math.h>

float Kp=0.1, Ki=0, Kd=0.5, Hz=1000;
int output_bits = 10;
bool output_signed = true;
int leftMem = 3;
int rightMem = 2;
Servo servoLeft;
Servo servoRight;

FastPID myPID(Kp, Ki, Kd, Hz, output_bits, output_signed);
int inputPin = A0;                             
int pwmPin = 3;

Actuator actuators[NUM*3];
float phases[NUM*3];

double PHASE_TEMP = 0.0;


Brain b;

void setup() {
  Serial.begin(9600);
  pinMode(inputPin, INPUT);
  pinMode(pwmPin, OUTPUT);
  pinMode(leftMem,OUTPUT);
  pinMode(rightMem,OUTPUT);
  servoLeft.attach(4);
  servoRight.attach(5);


 //printTables();

  //Create 15 Oscillators
  for(int i = 0; i < NUM*3; i++){
    actuators[i].index = i;
    actuators[i].phase = init_phases[i];
  }

  cli(); //disable interrupts 
  //set timer2 interrupt at 1000Hz
  //Timer2 interrupt is for transmitting 
  TCCR2A = 0;// set entire TCCR2A register to 0
  TCCR2B = 0;// same for TCCR2B
  TCNT2  = 0;//initialize counter value to 0
  // set compare match register for 1000hz increments
  OCR2A = 249;// = (16*10^6) / (1000*64) - 1 (must be <256)
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  // Set CS22 bit for 64 prescaler
  TCCR2B |= (1 << CS22); 
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);

  sei(); //enable interrupts*/

}
void loop() {
  for(int i = 0; i < NUM*3; i++){
    float rel_phases [SIZE];
    if(i < 5){
      for(int j = 0; j < 5; j++){
        rel_phases[j] = phases[j];
      }
      rel_phases[5] = phases[i+5];
      rel_phases[6] = phases[i+10];
    }
    else if(i < 10){
      for(int j = 0; j < 5; j++){
        rel_phases[j] = phases[j+5];
      }
      rel_phases[5] = phases[i-5];
      rel_phases[6] = phases[i+5];
    }
    else{
      for(int j = 0; j < 5; j++){
        rel_phases[j] = phases[j+10];
      }
      rel_phases[5] = phases[i-10];
      rel_phases[6] = phases[i-5];
    }
    
    float curr_phase = actuators[i].update_phase(rel_phases);
    phases[i] = curr_phase;
    
    if(i == 0){
      /*int expected = curr_phase*1024;
      int actual = analogRead(inputPin);
      //actual = 200;
      int output = myPID.step(expected, actual);
      printPID(expected, actual, output);
      //Serial.print(actual);
      //Serial.println(output);
      pwmWrite(pwmPin, output);
      if(output < -20){
        digitalWrite(leftMem,HIGH);
      }
      else{
        digitalWrite(leftMem,LOW);
      }*/
      
      int expected = cos(curr_phase*2*M_PI)*60;// placeholder for actual CPG phase
      PHASE_TEMP = 15*cos(3*curr_phase*2*M_PI);
      
      int actual = analogRead(inputPin);
      actual -= 495;
      int output = myPID.step(expected, actual);
      printPID(expected, actual, output);
      //Serial.print(actual);
      //Serial.println(output);
      pwmWrite(pwmPin, output);
      // TODO switch back to output
      actual = 0;
      if (/*output > 2*/ expected - actual > 5){
        digitalWrite(leftMem,HIGH);
      } else {
        digitalWrite(leftMem,LOW);
      }
      if (/*output < -2*/ expected - actual < -5) {
        digitalWrite(rightMem,HIGH);
      } else {
        digitalWrite(rightMem,LOW);
      }
    }
    delay(5);
    //graphPhases(i, curr_phase, true, false, false);
  } 

  //cycle_phase2();
}

void cycle_phase2() {
  PHASE_TEMP += 0.2;
  if (PHASE_TEMP >= M_PI*2) {
    PHASE_TEMP -= M_PI*2;
  }
 
}




ISR(TIMER2_COMPA_vect){//timer2 interrupt 1kHz 
  static int timer2_cnt = 0;
  timer2_cnt++;
  if (timer2_cnt == 5) {
    timer2_cnt = 0;
    // actual content
    int angle = PHASE_TEMP;//(int)(30*cos(PHASE_TEMP));
    angle += 90;
    servoLeft.write(angle);
    servoRight.write(angle);
  }
}




///////////////////////////////// HELPER FUNCTIONS /////////////////////////////////
void printPID(int expected, int actual, int output){
    Serial.print(F("Expected: "));
    Serial.print(expected);
    Serial.print(F("     Actual: "));
    Serial.print(actual);
    Serial.print(F("     Output: "));
    Serial.println(output);   
}


void graphPhases(int i, float curr_phase, bool seg1, bool seg2, bool seg3){
  if(seg1 && 0 <= i && i <= 4){
    Serial.print(10*curr_phase);
  }
  if(seg2 && 5 <= i && i <= 9){
    Serial.print(10*curr_phase);    
  }
  if(seg3 && 10 <= i && i <= 14){
    Serial.print(10*curr_phase);    
  }

  if(seg3){
    i != 14 ? Serial.print(" ") : Serial.println();        
  }
  else if(seg2){
    i != 9 ? Serial.print(" ") : Serial.println();
  }
  else if(seg1){
    i != 4 ? Serial.print(" ") : Serial.println();    
  }
  
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

      if(((i==1 || i==6 || i==11) && (j==0)) || 
         ((i==2 || i==7 || i==12) && (j==0 || j==1)) ||
         ((i==3 || i==8 || i==13) && (j==1 || j==2)) ||
         ((i==4 || i==9 || i==14) && (j==2 || j==3))){
          
          fracpart *= -1;
      }

      
      Serial.print(fracpart,4);
      Serial.print(F(" "));
    }
    Serial.println();
  }
  Serial.println(F(" ")); 
}
