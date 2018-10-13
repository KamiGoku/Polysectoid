// Using Serial Communcation to Send Data Between Arduinos
// Connect pins TX/RX of one Arduino to RX/TX of the other
// The Ground line has to be common for both Arduinos (connect them)


#include <Pattern.h>
#include <BasicLinearAlgebra.h>
#include <math.h>

int sensorPin = A0;   // Select the input pin for the potentiometer

void setup() {
  pinMode(sensorPin,INPUT);
  Serial.begin(9600);
  
}


void loop() {

  char sendVal[5] = "0000";
  itoa(analogRead(sensorPin),sendVal,10);
  
  Serial.write(sendVal,5);          // Sends Data

  char recVal[5];
  Serial.readBytes(recVal,5);
  Serial.println(recVal);           // Display the received value

  
  delay(10);
  
}
