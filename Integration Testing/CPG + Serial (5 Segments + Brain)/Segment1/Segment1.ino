#include "cpg.cpp"  

Actuator actuators [3];    // Oscillators 0, 5, 10

void setup() {
  Serial.begin(9600);

  // Hardcoding initial phases
  actuators[0].phase = 0.1;
  actuators[1].phase = 0.42;
  actuators[2].phase = 0.73;

  // IMPORT ALL FIELDS FOR EACH ACTUATOR HERE
  while(1){           // Break out once all 3 actuatos have received all data from brain


    
  }

  delay(1000);      // Chill for a sec to make sure all other arduinos are ready
}

void loop() {
    // Step 1: Each arduino sends their phase to 7 other arduinos
    for(int i = 0; i < 3; i++){
      if(i == 0){
        actuators[0].neighbor_phases[0] = actuators[i].phase;
        actuators[1].neighbor_phases[5] = actuators[i].phase;
        actuators[2].neighbor_phases[5] = actuators[i].phase;
        sendData(0,actuators[i].phase);
      }
      else if(i == 1){
        actuators[0].neighbor_phases[5] = actuators[i].phase;
        actuators[1].neighbor_phases[0] = actuators[i].phase;
        actuators[2].neighbor_phases[6] = actuators[i].phase;
        sendData(5,actuators[i].phase);        
      }
      else{
        actuators[0].neighbor_phases[6] = actuators[i].phase;
        actuators[1].neighbor_phases[6] = actuators[i].phase;
        actuators[2].neighbor_phases[0] = actuators[i].phase;
        sendData(10,actuators[i].phase);          
      }
      
    }

    // Step 2: Wait until each arduino has gotten phase data from the other 7 arduinos
    for(int i = 0; i < 3; i++){
      bool ready = false;
      while(!ready){
        // Read phase data here
        receiveData();

        
        ready = true;
        for(int j = 0; j < NUM; j++){
          if(actuators[i].neighbor_phases[j] == 0){ready = false;};
        }
      }
    }
    

    // Step 3: Update the phases of each arduino
    for(int i = 0; i < 3; i++){
      actuators[i].update_phase();
    }

    //Serial.print( (float) phase );    
}

// Send phase data to oscillator index
void sendData(int index, float phase){


  
}

// Receieve phase data from other oscillators
void receiveData(){
  
}

