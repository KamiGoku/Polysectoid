#include <Pattern.h>
#include <BasicLinearAlgebra.h>
#include <SoftwareSerial.h>
#include <math.h>


int sensorPin = A0;   // Select the input pin for the potentiometer


void setup() {
  pinMode(sensorPin,INPUT);
  Serial.begin(9600);
}

void loop() {

  // Sending Data
  char send_packet [5];
  int adcVal = analogRead(sensorPin);
  send_packet[4] = '\n';
  send_packet[3] = (adcVal % 10)+0x30;
  adcVal = adcVal/10;
  send_packet[2] = (adcVal % 10)+0x30;
  adcVal = adcVal/10;
  send_packet[1] = (adcVal % 10)+0x30;
  adcVal = adcVal/10;
  send_packet[0] = (adcVal % 10) + 0x30;

  // Formatting Data
  /*if(adcVal < 10){
    send_packet[4] = send_packet[0];
    send_packet[3] = '0';
    send_packet[2] = '0';
    send_packet[1] = '0';
  }
  else if (adcVal < 100){
    send_packet[4] = send_packet[1];
    send_packet[3] = send_packet[0];
    send_packet[2] = '0';
    send_packet[1] = '0';
  }
  else if(adcVal < 1000){
    send_packet[4] = send_packet[2];
    send_packet[3] = send_packet[1];
    send_packet[2] = send_packet[0];
    send_packet[1] = '0';
  }
  else {
    send_packet[4] = send_packet[3];
    send_packet[3] = send_packet[2];
    send_packet[2] = send_packet[1];
    send_packet[1] = send_packet[0];
  }*/
  
  //send_packet[0] = '0';
  Serial.write(send_packet,5); 
  //Serial.println(adcVal);

  delay(10);
  
}

