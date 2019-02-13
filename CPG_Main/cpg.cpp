#include <math.h>
#define SIZE 7
#define PI 3.14159265358979323846

class Actuator {
public: 
	

	double weights [SIZE];		// Weights for neighboring arduinos (0-4 -> vertical connection) (5,6 -> horizontal connections)
	double int_freq;		// Intrinsic Frequency
	double bias [SIZE];		// Bias for neighboring arduinos
	double tau;			// Frequency parameter
	double phase;			// Current phase of this actuator 

	Actuator(double Weights[], double Int_freq, double Bias [], double Tau) {
		int_freq = Int_freq;
		tau = Tau;
		
		for (int i = 0; i < SIZE; i++) {
			weights[i] = Weights[i];
			bias[i] = Bias[i];
		}
	}


	double update_phase(double neighbor_phases []) {
		double sum = 0.0;
		
		for (int i = 0; i < SIZE; i++) {
			sum += (weights[i] * sin(neighbor_phases[i] +  bias[i] - phase));
		}

		sum = ((2 * PI) + sum) / tau;
		
		double modVal = 2 * PI;
		phase = modf(sum + phase, &modVal);		

		return phase;
	}
};
