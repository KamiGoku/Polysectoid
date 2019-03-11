#include "brain.cpp"
#include <avr/pgmspace.h>




//template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; } //To make the " Serial << output " syntax possible


  // Brain Variables  
const float bias_pat_before[3][numBefore] PROGMEM = {{0.1, 0.1},
                                                     {0.1, 0.1},
                                                     {0.1, 0.1}};
const float bias_pat_after[3][numAfter] PROGMEM = {{0.1, 0.1},
                                                   {0.1, 0.1},
                                                   {0.1, 0.1}};

const float weight_pat_before [3][numBefore] PROGMEM = {{0.008, 0.004},
                                                        {0.02, 0.01},
                                                        {0.008, 0.004}};
const float weight_pat_after [3][numAfter] PROGMEM = {{0.008, 0.004},
                                                      {0.02, 0.01},
                                                      {0.008, 0.004}};

const float cross_weight [3][2] PROGMEM = {{0.0003, 0.0003},
                                           {0.0003, 0.0003},
                                           {0.0003, 0.0003}};
                             
const float cross_bias [3][2] PROGMEM = {{0, 0},
                                         {0, 0},
                                         {0, 0}};

const float int_freq[15] PROGMEM = {0.02,0.02,0.02,0.02,0.02, 0.01, 0.01, 0.01, 0.01, 0.01, 0.02, 0.02, 0.02 , 0.02, 0.02 };

const float init_phases [] PROGMEM = {.1, .7, .8, .23, .6, .42, .7, .9, .8, .6, .73, .7, .5, .44, .6};


Actuator actuators[NUM*3];
float phases[NUM*3];


void setup() {
  Serial.begin(9600);
      
  
  Pattern p = Pattern();
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 2; j++){
        p.bias_pat_before[i][j] = pgm_read_float(&bias_pat_before[i][j]);
        p.bias_pat_after[i][j] = pgm_read_float(&bias_pat_after[i][j]);
        p.weight_pat_before[i][j] = pgm_read_float(&weight_pat_before[i][j]);
        p.weight_pat_after[i][j] = pgm_read_float(&weight_pat_after[i][j]);
        p.cross_weight[i][j] = pgm_read_float(&cross_weight[i][j]);
        p.cross_bias[i][j] = pgm_read_float(&cross_bias[i][j]);
    }
  }


  for(int i = 0; i < NUM*3; i++){
     p.int_freq[i] = int_freq[i];
  }


  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 2; j++){
      Serial.print(p.bias_pat_after[i][j], 4);
      Serial.print(F(" "));
    }
    Serial.println();

  }
  Serial.flush();

  
  Brain b = Brain(&p,1.0);

  Serial.println(F("asdf"));

  for(int i = 0; i < 5; i++){
    for(int j = 0; j < SIZE; j++){
      Serial.print(b.weights[i][j],4);
      Serial.print(F(" "));
    }
    Serial.println();
  }
  /*for(int i = 0; i<3; i++){
    for(int j = 0; j<2;j++){
      Serial.print(p.bias_pat_after[i][j],4);
      Serial.print(" ");
    }
    Serial.println();
  }
  Serial.flush();*/




  for(int i = 0; i < NUM*3; i++){
    float cur_weights[SIZE], cur_bias[SIZE];
    for(int j = 0; j < SIZE; j++){
      cur_weights[j] = b.weights[i][j];
      cur_bias[j] = b.bias[i][j];
    }
    //actuators[i] = Actuator(cur_weights, b.int_freq[i], cur_bias, b.tau, init_phases[i]);
  }

  
 
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
      //Serial.print(10*curr_phase);
      //Serial.print(" ");
    }
    if(i== 5){
      //Serial.print(10*curr_phase);
      //Serial.print(" ");      
    }
    if(i == 10){
      //Serial.println(10*curr_phase);
      //Serial.print(" ");
    }
    if(i == 14){
      //Serial.println(0);
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
