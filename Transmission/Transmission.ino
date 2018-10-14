// Using Serial Communcation (UART) to Send Data Between Arduinos
// SoftwareSerial library allows us to create virtual TX and RX using other GPIO pins 
// (but this will only work if each arduino is communicating with 7 or less)

// Hardware Configuration:
// Connect TX/RX pin of each arduino to the RX/TX pin of all other arduinos
// The Ground line has to be common for both Arduinos (connect all of them together)

// Software Protocol:
// Assign each arduino a virtual ID
// Sending: For each transmission each arduino must specify the target arduino's ID at the beginning of the transmission
// Receiving: Upon receiving a transmission, each arduino must verify if the data received was meant for it


#include <Pattern.h>
#include <BasicLinearAlgebra.h>
#include <SoftwareSerial.h>
#include <math.h>


int sensorPin = A0;   // Select the input pin for the potentiometer
SoftwareSerial serial1(2,3); // RX, TX
SoftwareSerial serial2(4,5); // RX, TX
SoftwareSerial serial3(6,7); // RX, TX 

void setup() {
  pinMode(sensorPin,INPUT);
  Serial.begin(9600);

  // Start the software serials
  serial1.begin(9600);
  serial2.begin(9600);
  serial3.begin(9600);
}


void loop() {

  // Sending Data
  char send_packet [5];
  int adcVal = analogRead(sensorPin);
  itoa(adcVal,send_packet,10);

  // Formatting Data
  if(adcVal < 10){
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
  }
  
  send_packet[0] = '0';
  Serial.write(send_packet,5); 


  // Read 5 bytes at a time
  if(Serial.available() >= 5){
    char rec_packet[5];
    Serial.readBytes(rec_packet, 5);
    rec_packet[4] = '\n';

    // Checking if data was meant for this arduino
    if(rec_packet[0] == '0'){
      // Perform some useful work
      Serial.write(rec_packet,5);           // Display the received value 
    }
    
  }

  delay(100);
  
}
