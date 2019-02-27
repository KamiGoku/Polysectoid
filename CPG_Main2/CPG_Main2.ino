#include "cpg.cpp"
#include "brain.cpp"

template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; } //To make the " Serial << output " syntax possible


float phases[NUM*3];


// Brain Variables
float bias_pat_before [3][numBefore] = {{0.1, 0.1},
                                        {0.1, 0.1},
                                        {0.1, 0.1}};
float bias_pat_after[3][numAfter]= {{0.1, 0.1},
                                   {0.1, 0.1},
                                   {0.1, 0.1}};

float weight_pat_before [3][numBefore] = {{0.008, 0.004},
                                          {0.02, 0.01},
                                          {0.008, 0.004}};
float weight_pat_after [3][numAfter] = {{0.008, 0.004},
                                        {0.02, 0.01},
                                        {0.008, 0.004}};

float cross_weight [3][2] = {{0.0003, 0.0003},
                             {0.0003, 0.0003},
                             {0.0003, 0.0003}};
                             
float cross_bias [3][2] = {{0, 0},
                           {0, 0},
                           {0, 0}};

float int_freq[15] = {0.02,0.02,0.02,0.02,0.02, 0.01, 0.01, 0.01, 0.01, 0.01, 0.02, 0.02, 0.02 , 0.02, 0.02 };

Actuator actuators[NUM*3];
void setup() {
  Serial.begin(9600);

  
  Pattern p = Pattern();
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 2; j++){
        p.bias_pat_before[i][j] = bias_pat_before[i][j];
        p.bias_pat_after[i][j] = bias_pat_after[i][j];
        p.weight_pat_before[i][j] = weight_pat_before[i][j];
        p.weight_pat_after[i][j] = weight_pat_after[i][j];
        p.cross_weight[i][j] = cross_weight[i][j];
        p.cross_bias[i][j] = cross_bias[i][j];
    }
  }

  for(int i = 0; i < NUM*3; i++){
     p.int_freq[i] = int_freq[i];
 
  }

 //Serial << 'a';


  
  Brain b = Brain(p,1);

  float phases [] = {.1, .7, .8, .23, .6, .42, .7, .9, .8, .6, .73, .7, .5, .44, .6};
  
  for(int i = 0; i < NUM*3; i++){
    float cur_weights[SIZE], cur_bias[SIZE];
    for(int j = 0; j < SIZE; j++){
      cur_weights[j] = b.weights[i][j];
      cur_bias[j] = b.bias[i][j];
    }
    actuators[i] = Actuator(cur_weights, b.int_freq[i], cur_bias, b.tau, phases[i]);
  }

    for(int i = 0; i < NUM*3; i++){
    for(int j = 0; j < SIZE; j++){
      Serial.print(b.bias[i][j]);
      Serial.print(" ");
    }
    Serial.println();
  }
  Serial.println();
  Serial.println();
 
  //Serial << 'b';
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
