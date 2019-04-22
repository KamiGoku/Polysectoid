#include "brain.cpp"
Brain b;

void setup() {
  Serial.begin(9600);

  delay(3000);
  for(int i = 0; i < NUM*3; i++){
    float weights_i[SIZE], biases_i[SIZE], int_freq_i, init_phase_i, tau_i; // SEND ALL OF THESE TO OSCILLATOR i RAIYAN :)

    for(int j = 0; j < SIZE; j++){
      double weight,bias;
      bias = modf(b.weights_bias[i][j], &weight);
      weight = weight/10000;
      
      if(((i==1 || i==6 || i==11) && (j==0)) || 
         ((i==2 || i==7 || i==12) && (j==0 || j==1)) ||
         ((i==3 || i==8 || i==13) && (j==1 || j==2)) ||
         ((i==4 || i==9 || i==14) && (j==2 || j==3))){
        bias *= -1;
      }
      
      weights_i[j] = weight;
      biases_i[j] = bias;
    }

    int_freq_i = int_freq[i];
    init_phase_i = init_phases[i];
    tau_i = tau;  
    
    //\t character to start - 1
    //'b' to specify brain (alternatively 's' will specify segment elsewhere) - 2
    //space - 3
    //2 digits to specify oscillator number 00-14 - 5
    //space - 6
    //7 weights -- decimal precision of 4, so x.xxxx 6 chars with a space after so 7, 7*7 = 49 -- 55
    //7 biases -- same as weights, 6 chars+space, 7*7 = 49 -- 104
    //1 freq -- 6+1 = 7 -- 111
    //1 phase -- 6+1 = 7 -- 118
    //1 tau -- 6+0 (no space needed) = 6 -- 124
    //\n char -- 125

    Serial.write("\tb ");
    Serial.write(i/10+0x30);
    Serial.write(i%10+0x30);
    Serial.write(' ');
    char *ptr = (char*) malloc(SIZE*sizeof(char));
    for (int k = 0; k < SIZE; k++) {
      dtostrf(weights_i[k], 6, 4, ptr);
      ptr[6] = ' ';
      Serial.write(ptr,7);
    }
    for (int k = 0; k < SIZE; k++) {
      dtostrf(biases_i[k], 6, 4, ptr);
      ptr[6] = ' ';
      Serial.write(ptr,7);
    }
    dtostrf(int_freq_i, 6, 4, ptr);
    ptr[6] = ' ';
    Serial.write(ptr,7);
    dtostrf(init_phase_i, 6, 4, ptr);
    ptr[6] = ' ';
    Serial.write(ptr,7);
    dtostrf(tau_i, 6, 4, ptr);
    ptr[6] = '\n';
    Serial.write(ptr,7);
    free(ptr);
    delay(1000);
  }

  
  while(1);
}

void loop() {

  

  
}
