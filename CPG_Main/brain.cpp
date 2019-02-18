#include "pattern.cpp"
#define NUM 5
#define SIZE (NUM + 2)



class Brain {
public:
	float weights[NUM * 3][SIZE];			// Weights for neighboring arduinos
	float int_freq [NUM * 3];				// Intrinsic Frequency
	float bias[NUM * 3][SIZE];				// Bias for neighboring arduinos
	float tau;								// Frequency parameter

	Brain(Pattern pattern, float Tau) {
		tau = Tau;
		
		//Weight calculation
		for (int k = 0; k < 3; k++) {
			float row[NUM];
			row[0] = pattern.weight_pat_after[k][0];
			row[1] = pattern.weight_pat_after[k][1];
			row[2] = 0;
			row[3] = pattern.weight_pat_before[k][0];
			row[4] = pattern.weight_pat_before[k][1];


			for (int i = k*5; i < (k+1)*5; i++) {
				weights[i][5] = pattern.cross_weight[k][0];
				weights[i][6] = pattern.cross_weight[k][1];

				for (int j = 0; j < 5; j++) {
					if (i == 0 && j < 3) {
						weights[i][j] = row[j + 2];
					}
					else if (i == 1 && j < 4) {
						weights[i][j] = row[j + 1];
					}
					else if (i == 2 && j < 5) {
						weights[i][j] = row[j];
					}
					else if (i == 3 && j > 0) {
						weights[i][j] = row[j - 1];
					}
					else if (i == 4 && j > 1) {
						weights[i][j] = row[j - 2];
					}
				}
			}
		}

		


	}


};

int main() {

}
