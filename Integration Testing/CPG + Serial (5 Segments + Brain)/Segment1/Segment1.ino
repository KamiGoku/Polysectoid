#include "cpg.cpp"  

 Actuator actuators [3];    // Oscillators 0, 5, 10

void setup() {
  Serial.begin(9600);

  // Hardcoding initial phases
  actuators[0].phase = 0.1;
  actuators[1].phase = 0.42;
  actuators[2].phase = 0.73;

  // IMPORT ALL THE ABOVE VARIABLES HERE FROM BRAIN RAIYAN :)
  while(1){           // Break out once all 3 actuatos have received all data from brain


    
  }

  delay(1000);      // Chill for a sec to make sure all other arduinos are ready

}

void loop() {
    // Step 1: Each arduino sends their phase to 7 other arduinos
    for(int i = 0; i < 3; i++){
      
    }

    // Step 2: wait until each arduino has gotten phase data from the other 7 arduinos
    for(int i = 0; i < 3; i++){
      bool ready = false;
      while(!ready){
        ready = true;
        for(int j = 0; j < NUM; j++){
          if(actuators[i].neighbor_phases[j] == 0){ready = false;};
        }
      }
    }
    
    

    //Serial.print( (float) phase );  

  
}

// Send phase data to oscillator index
void sendData(int index, float phase){


  
}

