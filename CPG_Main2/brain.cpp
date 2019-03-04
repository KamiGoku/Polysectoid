#include "pattern.cpp"
#define NUM 5
#define SIZE (NUM + 2)



class Brain {
public:
	float weights[NUM * 3][SIZE];		// Weights for neighboring arduinos
	float int_freq [NUM * 3];				// Intrinsic Frequency
	float bias[NUM * 3][SIZE];			// Bias for neighboring arduinos
	float tau;								      // Frequency parameter

	Brain(Pattern pattern, float Tau) {
		tau = Tau;
		
		//Weight calculation
		for (int k = 0; k < 3; k++) {
			float weight_row[NUM], bias_row[NUM];
			/*weight_row[0] = pattern.weight_pat_after[k][0];
			weight_row[1] = pattern.weight_pat_after[k][1];
			weight_row[2] = 0;
			weight_row[3] = pattern.weight_pat_before[k][0];
			weight_row[4] = pattern.weight_pat_before[k][1];

			bias_row[0] = -1*pattern.bias_pat_after[k][0] - pattern.bias_pat_after[k][1];
			bias_row[1] = -1*pattern.bias_pat_after[k][0];
			bias_row[2] = 0;
			bias_row[3] = pattern.bias_pat_before[k][0];
			bias_row[4] = pattern.bias_pat_before[k][0] + pattern.bias_pat_before[k][1];*/
			


			for (int i = k*5; i < (k+1)*5; i++) {
				weights[i][5] = pattern.cross_weight[k][0];
				weights[i][6] = pattern.cross_weight[k][1];
        bias[i][5] = 0;
        bias[i][6] = 0;

       
        for (int w = NUM-1; w >= 3; w--){//before
          for (int x = 0; x < w; x++){
            weights[x+k*NUM][x+(NUM-w)] = pattern.weight_pat_before[k][x];
            bias[x+k*NUM][x+(NUM-w)] = pattern.bias_pat_before[k][x];
          }
        }
        for (int w = NUM-1; w >= 3; w--){//after
          for (int x = 0; x < w; x++){
            weights[x+(NUM-w)+k*NUM][x] = pattern.weight_pat_after[k][x];
            bias[x+(NUM-w)+k*NUM][x] = pattern.bias_pat_after[k][x];
          }
        }

				/*for (int j = 0; j < 5; j++) {
					if (i == 0 && j < 3) {
						weights[i][j] = weight_row[j + 2];
            bias[i][j] = bias_row[j+2];
					}
					else if (i == 1 && j < 4) {
						weights[i][j] = weight_row[j + 1];
            bias[i][j] = bias_row[j+1];
					}
					else if (i == 2 && j < 5) {
						weights[i][j] = weight_row[j];
            bias[i][j] = bias_row[j];
					}
					else if (i == 3 && j > 0) {
						weights[i][j] = weight_row[j - 1];
            bias[i][j] = bias_row[j-1];
					}
					else if (i == 4 && j > 1) {
						weights[i][j] = weight_row[j - 2];
            bias[i][j] = bias_row[j-2];
					}
				}*/
			}
		}

		for (int i = 0; i < 15; i++) {
			int_freq[i] = pattern.int_freq[i];
		}

	}
 
};
