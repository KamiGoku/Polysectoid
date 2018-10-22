#include <Pattern.h>
#include <BasicLinearAlgebra.h>
#include <SoftwareSerial.h>
#include <math.h>

void setup() {
  Serial.begin(9600);

}

void loop() {
  
  // Read 5 bytes at a time
  if(Serial.available() >= 5){
    char rec_packet[5];
    Serial.readBytes(rec_packet, 5);
    //rec_packet[4] = '\n';

    // Checking if data was meant for this arduino
    if(rec_packet[0] == '0'){
      // Perform some useful work
      Serial.write(rec_packet,5);           // Display the received value 
      Serial.write('\n');
    }
    
  }

  delay(1);
}
