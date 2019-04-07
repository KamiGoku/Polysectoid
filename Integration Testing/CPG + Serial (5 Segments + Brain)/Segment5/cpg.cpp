#include <math.h>
#define NUM 5
#define SIZE NUM + 2

class Actuator {
public:
 

	float weights[SIZE];		// Weights for neighboring arduinos (0-4 -> vertical connection) (5,6 -> horizontal connections)
	float int_freq;			    // Intrinsic Frequency
	float biases[SIZE];			// Bias for neighboring arduinos
	float tau;					    // Frequency parameter
	float phase;				    // Current phase of this actuator 
  int index;              // ID number of Oscillator

  float neighbor_phases[SIZE] = {0}; // Phases of neighboring arduinos

  Actuator(){}

	float update_phase() {
    float sum = 0.0;

    for (int i = 0; i < SIZE; i++) {
      double weight,bias;
      bias = modf(biases[i], &weight);
      weight = weight/10000.0;

      if(((index==1 || index==6 || index==11) && (i==0)) || 
         ((index==2 || index==7 || index==12) && (i==0 || i==1)) ||
         ((index==3 || index==8 || index==13) && (i==1 || i==2)) ||
         ((index==4 || index==9 || index==14) && (i==2 || i==3))){
          
          bias *= -1;
      }
      sum += weight * sin((double) ((neighbor_phases[i] - phase) * 2 * M_PI - bias));
    }

    sum = ((2 * M_PI * int_freq) + sum) / (2 * M_PI * tau);

    double modVal = 1;
    phase = modf(sum + phase, &modVal);

    return (float) phase;
	}
};
