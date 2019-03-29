#include "brain.cpp"
#include <AltSoftSerial.h>
#include "Serial_B.h"
Brain b;
AltSoftSerial brainSerial;

void setup() {
  Serial.begin(9600);
  while(!Serial);
  brainSerial.begin(9600);

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
    char packet[125];
    packet[0] = '\t';
    packet[1] = 'b';
    packet[2] = ' ';
    packet[3] = (i/10)+0x30;
    packet[4] = (i%10)+0x30;
    packet[5] = ' ';
    int packet_idx = 6;
    for (int i = 0; i < SIZE; i++) {
      dtostrf(weights_i[i], 6, 4, packet+packet_idx);
      packet[packet_idx+6] = ' ';
      packet_idx += 7;
    }
    for (int i = 0; i < SIZE; i++) {
      dtostrf(biases_i[i], 6, 4, packet+packet_idx);
      packet[packet_idx+6] = ' ';
      packet_idx += 7;
    }
    dtostrf(int_freq_i, 6, 4, packet+packet_idx);
    packet[packet_idx+6] = ' ';
    packet_idx += 7;
    dtostrf(init_phase_i, 6, 4, packet+packet_idx);
    packet[packet_idx+6] = ' ';
    packet_idx += 7;
    dtostrf(tau, 6, 4, packet+packet_idx);
    packet[packet_idx+6] = '\n';
    
    sendData(brainSerial, packet, 125);
  }
}

void loop() {



  
  

  
}
