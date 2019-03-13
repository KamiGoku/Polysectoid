#include <math.h>
#include "brain.cpp"


extern Brain b;



class Actuator {
public:
 

	//float weights[SIZE];		// Weights for neighboring arduinos (0-4 -> vertical connection) (5,6 -> horizontal connections)
	//float int_freq;			    // Intrinsic Frequency
	//float bias[SIZE];			  // Bias for neighboring arduinos
	//float tau;					    // Frequency parameter
	float phase;				      // Current phase of this actuator 
  int index;

	Actuator(float Weights[], float Int_freq, float Bias[], float Tau, float Phase) {
		/*int_freq = Int_freq;
		tau = Tau;
    phase = Phase;
		for (int i = 0; i < SIZE; i++) {
			weights[i] = Weights[i];
			bias[i] = Bias[i];
		}*/
	}
 Actuator(){}



	float update_phase(float neighbor_phases[]) {
		float sum = 0.0;

		for (int i = 0; i < SIZE; i++) {
			//sum += (weights[i] * sin((double) ((neighbor_phases[i] - phase) * 2 * M_PI - bias[i])));
      double intpart,fracpart;
      fracpart = modf(b.weights_bias[index][i], &intpart);
      intpart = intpart/10000;

      if(index == 0 || index == 1 || index == 5 || index == 6 || index == 10 || index == 11){
        fracpart *= -1;
      }

      sum += (intpart * sin((double) ((neighbor_phases[i] - phase) * 2 * M_PI - fracpart)));
		}

		//sum = ((2 * M_PI * int_freq) + sum) / (2 * M_PI * tau);
   sum = ((2 * M_PI * int_freq[index]) + sum) / (2 * M_PI * tau);

		double modVal = 1;
		phase = modf(sum + phase, &modVal);

		return (float) phase;
	}
};
