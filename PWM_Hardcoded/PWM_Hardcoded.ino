#include <Servo.h>

Servo servoLeft;
Servo servoRight;

void setup() {
  Serial.begin(9600);
  servoLeft.attach(A1);
  servoRight.attach(A2);
}


void loop() {
  rotate_back_forth();
  delay (1000);
}



void rotate_back_forth(){
  for(int angle = 5; angle < 180; angle++){
    servoLeft.write(angle);
    delay(10);
  }

  for(int angle = 180; angle > 5; angle--){
    servoLeft.write(angle);
    delay(10);
  }
  
}

