#include <math.h>
#define NUM 5
#define SIZE NUM + 2

class Actuator {
public:
 

	float weights[SIZE];		// Weights for neighboring arduinos (0-4 -> vertical connection) (5,6 -> horizontal connections)
	float int_freq;			    // Intrinsic Frequency
	float bias[SIZE];			  // Bias for neighboring arduinos
	float tau;					    // Frequency parameter
	float phase;				    // Current phase of this actuator 
  int index;              // ID number of Oscillator

  float neighbor_phases[SIZE] = {0}; // Phases of neighboring arduinos

 Actuator(){}

	float update_phase(float neighbor_phases[]) {
		float sum = 0.0;

		for (int i = 0; i < SIZE; i++) {
			//sum += (weights[i] * sin((double) ((neighbor_phases[i] - phase) * 2 * M_PI - bias[i])));
      double intpart,fracpart;
      fracpart = modf(neighbor_phases[i], &intpart);
      intpart = intpart/10000;

      if(index == 0 || index == 1 || index == 5 || index == 6 || index == 10 || index == 11){
        fracpart *= -1;
      }

      sum += (intpart * sin((double) ((neighbor_phases[i] - phase) * 2 * M_PI - fracpart)));
		}

    sum = ((2 * M_PI * int_freq) + sum) / (2 * M_PI * tau);

		double modVal = 1;
		phase = modf(sum + phase, &modVal);

		return (float) phase;
	}
};
