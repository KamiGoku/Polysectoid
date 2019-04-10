#include <Servo.h>

Servo servoLeft;
Servo servoRight;

void setup() {
  Serial.begin(9600);
  servoLeft.attach(4);
  servoRight.attach(5);
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
  //cycle_phase();
  rotate_offset();
}



void rotate_back_forth(){
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
  double phase = 0.0;
  while(1) {
    phase += 0.05;
    int angle = (int)(30*cos(phase));
    angle += 90;
    servoLeft.write(angle);
    servoRight.write(180-angle);
    delay(5);
  }
}

