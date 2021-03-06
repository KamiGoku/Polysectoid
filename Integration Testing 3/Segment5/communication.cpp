#include <RingBuf.h>
#include <AltSoftSerial.h>
#include "Serial_IB.h"
#include "communication.h"

bool actuators_set[3] = {false,false,false};
extern RingBuf *brain_buf;
extern RingBuf *seg_buf;
extern AltSoftSerial altSerial;
extern Actuator actuators[3];
int actuator_count = 0;

void setActuators(char packet[124], int &bi, int actuator_num) {
  //2 digits to specify oscillator number 00-14 - 5
  //space - 6 <--- we start off here
  //7 weights -- decimal precision of 4, so x.xxxx 6 chars with a space after so 7, 7*7 = 49 -- 55
  //7 biases -- same as weights, 6 chars+space, 7*7 = 49 -- 104
  //1 freq -- 6+1 = 7 -- 111
  //1 phase -- 6+1 = 7 -- 118
  //1 tau -- 6+0 (no space needed) = 6 -- 124
  //\n char -- 125
  //subtract all the above by 1 since \t isn't included
  //oscillators 0, 5, 10
  bi++;
  for (int i = 0; i < SIZE; i++) {
    actuators[actuator_num].weights[i] = atof(packet+bi);
    //1.2345 <-- start off pointing to 1, +6 to point at space after, +7 to point at next weight
    bi += 7;
  }
  for (int i = 0; i < SIZE; i++) {
    actuators[actuator_num].biases[i] = atof(packet+bi);
    bi += 7;
  }
  actuators[actuator_num].int_freq = atof(packet+bi);
  bi += 7;
  actuators[actuator_num].phase = atof(packet+bi);
  bi += 7;
  actuators[actuator_num].tau = atof(packet+bi);
  
  actuators_set[actuator_num] = true;
  
  return;
}


void getBrainData(){
   static int read_flag = 0;

  // IMPORT ALL THE ABOVE VARIABLES HERE FROM BRAIN RAIYAN :)
  while(!actuators_set[0] || !actuators_set[1] || !actuators_set[2] || actuator_count < 3){           // Break out once all 3 actuatos have received all data from brain
    while (brain_buf->isEmpty(brain_buf)) {
      readData(altSerial, read_flag, 1);
    }
    
    char *brain_packet = (char *) malloc(124*sizeof(char));
    brain_buf->pull(brain_buf, brain_packet);
    int brain_idx = 0;
    if (brain_packet[brain_idx] == 'b') {//just to verify
      brain_idx += 4;
      if (brain_packet[brain_idx-2] == '0' && brain_packet[brain_idx-1] == '4') {
        //Serial.write("ACTUATOR 04\n");
        actuator_count++;
        setActuators(brain_packet, brain_idx, 0);
      } else if (brain_packet[brain_idx-2] == '0' && brain_packet[brain_idx-1] == '9') {
        //Serial.write("ACTUATOR 09\n");
        actuator_count++;
        setActuators(brain_packet, brain_idx, 1);
      } else if (brain_packet[brain_idx-2] == '1' && brain_packet[brain_idx-1] == '4') {
        //Serial.write("ACTUATOR 14\n");
        actuator_count++;
        setActuators(brain_packet, brain_idx, 2);
      } else {
        actuator_count++;
      }
    }
    free(brain_packet);
  }

  //test to make sure we got the goods
  /*Serial.write("Weights:\n");
  for (int i = 0; i < SIZE; i++) {
    Serial.print(actuators[0].weights[i],4);
    Serial.write(", ");
    Serial.print(actuators[1].weights[i],4);
    Serial.write(", ");
    Serial.println(actuators[2].weights[i],4);
  }
  Serial.write("Biases:\n");
  for (int i = 0; i < SIZE; i++) {
    Serial.print(actuators[0].biases[i],4);
    Serial.write(", ");
    Serial.print(actuators[1].biases[i],4);
    Serial.write(", ");
    Serial.println(actuators[2].biases[i],4);
  }
  Serial.write("Freq:\n");
  Serial.print(actuators[0].int_freq,4);
  Serial.write(", ");
  Serial.print(actuators[1].int_freq,4);
  Serial.write(", ");
  Serial.println(actuators[2].int_freq,4);
  Serial.write("Phase:\n");
  Serial.print(actuators[0].phase,4);
  Serial.write(", ");
  Serial.print(actuators[1].phase,4);
  Serial.write(", ");
  Serial.println(actuators[2].phase,4);
  Serial.write("Tau:\n");
  Serial.print(actuators[0].tau,4);
  Serial.write(", ");
  Serial.print(actuators[1].tau,4);
  Serial.write(", ");
  Serial.println(actuators[2].tau,4);*/
}

// Sends data to all other 4 arduinos.
// from: index of actuator that corresponds to phase 
void sendData(int from, float phase){
  char *ptr = (char*) malloc(13*sizeof(char));
  ptr[0] = 's';
  ptr[1] = ' ';
  ptr[2] = (from/10)+0x30;
  ptr[3] = (from%10)+0x30;
  ptr[4] = ' ';
  dtostrf(phase, 6, 4, ptr+5);
  ptr[12] = '\n';
  //Segments 1-4 alt TX
  //sendData(Serial, ptr, 14);
  sendData(altSerial, ptr, 14);
  free(ptr);
  
}


// Blocks until all 3 oscillators get data from 7 other arduinos
// Actuator 0 is listening for actuators 0,1,2,3
// Actuator 1 is listening for actuators 5,6,7,8
// Actuator 2 is listening for actuators 10,11,12,13
void readData(){
  //listening from alt RX for segments1-4
  static int read_flag = 0;

  //bits 0-3, 5-8, 10-13 will be set when all data received
  //0011 1101 1110 1111 -> 0x3DEF
  uint16_t listening = 0;

  while(listening != 0x3DEF){
    while(seg_buf->isEmpty(seg_buf)){
      readData(altSerial, read_flag, 1);
    }
    while(!seg_buf->isEmpty(seg_buf)){
      char *phase_data = (char *) malloc(13*sizeof(char));
      seg_buf->pull(seg_buf, phase_data);
      if (phase_data[0] == 's'){
        int from = atoi(phase_data+2);
        //I don't remember where the phases go so I'm gonna assume
        //e.g. in 4's case: self,0,1,2,3,9,14
        if (from < 5){
          actuators[0].neighbor_phases[from+1] = atof(phase_data+5);
        } else if (from < 10){
          actuators[1].neighbor_phases[from-4] = atof(phase_data+5);
        } else {
          actuators[2].neighbor_phases[from-9] = atof(phase_data+5);
        }
        listening |= (1 << from);
      }
      free(phase_data);
    }
  }

  
}
