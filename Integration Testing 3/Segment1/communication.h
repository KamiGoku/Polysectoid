#include "cpg.cpp"

// Blocks until all 3 oscillators get data from the brain
void getBrainData();


// Sends data to all other arduinos.
// from: index of oscillator that corresponds to phase 
void sendPhaseData(int from, float phase);


// Blocks until all 3 oscillators get data from 7 other arduinos
// Actuator 0 is listening for actuators 1,2,3,4
// Actuator 1 is listening for actuators 6,7,8,9
// Actuator 2 is listening for actuators 11,12,13,14
void readPhaseData();
