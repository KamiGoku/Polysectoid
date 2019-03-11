//#include "brain.cpp"
#include "cpg.cpp"

Actuator actuators[NUM*3];
float phases[NUM*3];

Brain b;

void setup() {
  Serial.begin(9600);
  


  /*Serial.println(F("WEIGHT TABLE:"));
  for(int i = 0; i < 15; i++){
    for(int j = 0; j < SIZE; j++){
      Serial.print(((int)b.weights_bias[i][j])/10000.0,4);
      Serial.print(F(" "));
    }
    Serial.println();
  }
  Serial.println(F("done"));

  Serial.println(F("BIAS TABLE:"));
  for(int i = 0; i < 15; i++){
    for(int j = 0; j < SIZE; j++){
      double intpart,fracpart;
      fracpart = modf(b.weights_bias[i][j], &intpart);
      Serial.print(fracpart,4);
      Serial.print(F(" "));
    }
    Serial.println();
  }
  Serial.println(F("done"));*/


  // Simulate 15 Oscillators
  for(int i = 0; i < NUM*3; i++){



    //actuators[i].int_freq = int_freq[i];
    actuators[i].index = i;
    //actuators[i].phase = init_phases[i];
    //actuators[i] = Actuator(weights_row, int_freq[i], weights_row/*bias_row*/, b.tau, init_phases[i]);
  }

  
 // Serial.println(F("setup done"));

}
void loop() {
  for(int i = 0; i < NUM*3; i++){
    float rel_phases [SIZE];
    if(i < 5){
      for(int j = 0; j < 5; j++){
        rel_phases[j] = phases[j];
      }
      rel_phases[6] = phases[i+5];
      rel_phases[7] = phases[i+10];
    }
    else if(i <= 10){
      for(int j = 0; j < 5; j++){
        rel_phases[j] = phases[j+5];
      }
      rel_phases[6] = phases[i+5];
      rel_phases[7] = phases[i-5];
    }
    else{
      for(int j = 0; j < 5; j++){
        rel_phases[j] = phases[j+10];
      }
      rel_phases[6] = phases[i-10];
      rel_phases[7] = phases[i-5];
    }
    
    float curr_phase = actuators[i].update_phase(rel_phases);

    /*if(i == NUM*3-1){
      Serial.println(10*curr_phase);
      Serial.print(" ");
    }
    else{
      Serial.print(10*curr_phase);     
      Serial.print(" "); 
    }*/

    if(i == 0){
      Serial.print(10*curr_phase);
      Serial.print(" ");
    }
    if(i== 5){
      Serial.print(10*curr_phase);
      Serial.print(" ");      
    }
    if(i == 10){
      Serial.println(10*curr_phase);
      Serial.print(" ");
    }
    if(i == 14){
      Serial.println();
    }
    
    /*Serial.print("ID: ");
    Serial.print(i);
    Serial.print(", ");
    Serial.println(curr_phase);*/
    //Serial.print(2);
    //Serial.print(" ");
    //Serial.println(1);
  }



  
  

}
