#include <Servo.h>

#define pi2 6.28318530718
Servo servoLeft;
Servo servoRight;
int leftMem = A0;
int rightMem = A1;

double phase = 0.0;

void setup() {
  Serial.begin(9600);
  servoLeft.attach(4);
  servoRight.attach(6);
  pinMode(leftMem,OUTPUT);
  pinMode(rightMem,OUTPUT);

  digitalWrite(leftMem,HIGH);
  
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

ISR(TIMER2_COMPA_vect){//timer2 interrupt 1kHz 
  static int timer2_cnt = 0;
  timer2_cnt++;
  if (timer2_cnt == 5) {
    timer2_cnt = 0;
    // actual content
    int angle = (int)(15*cos(phase));
    angle += 90;
    servoLeft.write(angle);
    servoRight.write(angle);
  }
}

void loop() {
  //rotate_back_forth();
  /*int angle = rotate_offset();
  uint32_t startTime = millis();
  uint32_t endTime = startTime+500;
  if (endTime < startTime) {
    uint32_t test_millis = millis();
    while(test_millis+500 < test_millis)
      test_millis = millis();
    startTime = millis();
    endTime = startTime+500;
  }
  do {
    servoLeft.write(angle);
    servoRight.write(180-angle);
    
  } while (endTime > millis());
  //delay (500);*/
  cycle_phase2();
  //rotate_offset();
}



/*void rotate_back_forth(){
  for(int angle = 60; angle < 120; angle++){
    servoLeft.write(angle);
    servoRight.write(angle);
    delay(5);
  }

  for(int angle = 120; angle > 60; angle--){
    servoLeft.write(angle);
    servoRight.write(angle);
    delay(5);
  }
  
}

int rotate_offset(){
  int angle;
  for(angle = 60; angle <= 120; angle+=2){
    Serial.print(angle);
    Serial.print(" ");
    Serial.println(180-angle);
    servoLeft.write(angle);
    servoRight.write(180-angle);
    delay(5);
  }
  for(angle = 120; angle >= 60; angle-=2){
    servoLeft.write(angle);
    servoRight.write(180-angle);
    delay(5);
  }
  return angle;
}

void cycle_phase() {
  while(1) {
    phase += 0.05;
    if (phase > pi2) {
      phase -= pi2;
    }
    int angle = (int)(30*cos(phase));
    angle += 90;
    servoLeft.write(angle);
    servoRight.write(180-angle);
    delay(5);
  }
}*/

void cycle_phase2() {
  while(1) {
    phase += 0.05;
    if (phase > pi2) {
      phase -= pi2;
    }
    delay(5);
  }
}
