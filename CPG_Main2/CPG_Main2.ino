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

float int_freq[15] = {0.002,0.002,0.002,0.002,0.002, 0.001, 0.001, 0.001, 0.001, 0.001, 0.0005, 0.0005, 0.0005 , 0.0005, 0.0005 };

Actuator* actuators[15];
void setup() {
  Serial.begin(9600);

  
  Pattern *p = new Pattern();
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 2; j++){
        p->bias_pat_before[i][j] = bias_pat_before[i][j];
        p->bias_pat_after[i][j] = bias_pat_after[i][j];
        p->weight_pat_before[i][j] = weight_pat_before[i][j];
        p->weight_pat_after[i][j] = weight_pat_after[i][j];
        p->cross_weight[i][j] = cross_weight[i][j];
        p->cross_bias[i][j] = cross_bias[i][j];
    }
  }

  for(int i = 0; i < 15; i++){
     p->int_freq[i] = int_freq[i];
 
  }

 Serial << 'a';


  
  Brain *b = new Brain(p,1);
  
  for(int i = 0; i < NUM*3; i++){
    float cur_weights[SIZE], cur_bias[SIZE];
    for(int j = 0; j < SIZE; j++){
      cur_weights[j] = b->weights[i][j];
      cur_bias[j] = b->bias[i][j];
    }
    //actuators[i] = new Actuator(cur_weights,b->int_freq[i],cur_bias,b->tau,1.0/(i+1));
    actuators[i] = new Actuator();
  }
 
  Serial << 'c';
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
    
    float curr_phase = actuators[i]->update_phase(rel_phases);
    //Serial.print(curr_phase);
    //Serial.print(" ");
  }

  
  

}
