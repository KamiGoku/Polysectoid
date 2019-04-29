#include <AltSoftSerial.h>
#include <RingBuf.h>
#include "Serial_IB.h"
#include "communication.h" 

Actuator actuators [3];    // Oscillators 3, 8, 13
AltSoftSerial altSerial;
RingBuf *brain_buf = RingBuf_new(124 * sizeof(char), 1);
RingBuf *seg_buf = RingBuf_new(14 * sizeof(char), 10);

void setup() {
  Serial.begin(9600);
  while(!Serial);
  altSerial.begin(9600);

  // Hardcoding initial phases
  actuators[0].phase = 0.23;
  actuators[1].phase = 0.8;
  actuators[2].phase = 0.44;

  // Importing brain data
  getBrainData();       // Breaks out once all 3 actuators have received all data from brain

  delay(1000);      // Chill for a sec to make sure all other arduinos are ready
}

void loop() {
    // Step 1: Each arduino sends their phase to 7 other arduinos
    for(int i = 0; i < 3; i++){
      if(i == 0){
        actuators[0].neighbor_phases[0] = actuators[i].phase;
        actuators[1].neighbor_phases[5] = actuators[i].phase;
        actuators[2].neighbor_phases[5] = actuators[i].phase;
        sendData(3,actuators[i].phase);
      }
      else if(i == 1){
        actuators[0].neighbor_phases[5] = actuators[i].phase;
        actuators[1].neighbor_phases[0] = actuators[i].phase;
        actuators[2].neighbor_phases[6] = actuators[i].phase;
        sendData(8,actuators[i].phase);        
      }
      else{
        actuators[0].neighbor_phases[6] = actuators[i].phase;
        actuators[1].neighbor_phases[6] = actuators[i].phase;
        actuators[2].neighbor_phases[0] = actuators[i].phase;
        sendData(13,actuators[i].phase);          
      }
      
    }

    // Step 2: Wait until each arduino has gotten phase data from the other 7 arduinos
    readData();
    

    // Step 3: Update the phases of each arduino
    for(int i = 0; i < 3; i++){
      actuators[i].update_phase();
    }

    //Serial.print( (float) phase );    
}
