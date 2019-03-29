#include "brain.cpp"
Brain b;

void setup() {
  Serial.begin(9600);
}

void loop() {

  for(int i = 0; i < NUM*3; i++){
    float weights_i[SIZE], biases_i[SIZE], int_freq_i, init_phase_i, tau_i; // SEND ALL OF THESE TO OSCILLATOR i RAIYAN :)

    for(int j = 0; j < SIZE; j++){
      double weight,bias;
      bias = modf(b.weights_bias[i][j], &weight);
      weight = weight/10000;
      
      if(i == 0 || i == 1 || i == 5 || i == 6 || i == 10 || i == 11){
        bias *= -1;
      }
      
      weights_i[i] = weight;
      biases_i[i] = bias;
    }

    int_freq_i = int_freq[i];
    init_phase_i = init_phases[i];
    tau_i = tau;  
  }

  
  

  
}
