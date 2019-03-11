#include "pattern.cpp"
#include "cpg.cpp"


extern Actuator actuators[];

class Brain {
public:
	float weights[NUM][SIZE] = {{0}};	  // Weights for neighboring arduinos
	float int_freq [NUM*3] = {{0}};			// Intrinsic Frequency
	float bias[NUM][SIZE] = {{0}};		  // Bias for neighboring arduinos
	float tau;								          // Frequency parameter


	Brain(Pattern* pattern, float Tau) {
		tau = Tau;
    for(int i = 0; i < NUM*3; i++){
      int_freq[i] = pattern->int_freq[i];
    }

    for(int i = 0; i < 3; i++){
          calculate_weight(pattern, i);
          calculate_bias(pattern, i);
          for(int j = 0; j < 5; j++){
            actuators[(i*k) + j] = Actuator(cur_weights, b.int_freq[(i*k)+j], bias[j], b.tau, init_phases[i]);
          }

    }



	}

  void calculate_weight(Pattern *pattern, int inded){
    for (int k = index; k < index+1; k++) {
      float weight_row[NUM];
      
      weight_row[0] = pattern->weight_pat_after[k][1];
      weight_row[1] = pattern->weight_pat_after[k][0];
      weight_row[2] = 0;
      weight_row[3] = pattern->weight_pat_before[k][0];
      weight_row[4] = pattern->weight_pat_before[k][1];  
      
      for (int i = k*5; i < (k+1)*NUM; i++) {
        weights[i][5] = pattern->cross_weight[k][0];
        weights[i][6] = pattern->cross_weight[k][1];
        
        for (int j = 0; j < 5; j++) {
          if ((i == k*5) && j < 3) {
            weights[i][j] = weight_row[j + 2];
          }
          else if ((i == (k*5)+1) && j < 4) {
            weights[i][j] = weight_row[j + 1];
          }
          else if ((i == (k*5)+2) && j < 5) {
            weights[i][j] = weight_row[j];
          }
          else if ((i == (k*5)+3) && j > 0) {
            weights[i][j] = weight_row[j - 1];
          }
          else if ((i == (k*5)+4) && j > 1) {
            weights[i][j] = weight_row[j - 2];
          }
        }
      }
    }  
  }

  void calculate_bias(Pattern *pattern, int i){
    for (int k = 0; k < 1; k++) {
      float bias_row[NUM];
      
      bias_row[0] = -1*pattern->bias_pat_after[k][0] - pattern->bias_pat_after[k][1];
      bias_row[1] = -1*pattern->bias_pat_after[k][0];
      bias_row[2] = 0;
      bias_row[3] = pattern->bias_pat_before[k][0];
      bias_row[4] = pattern->bias_pat_before[k][0] + pattern->bias_pat_before[k][1];
      
      for (int i = k*5; i < (k+1)*NUM; i++) {
        bias[i][5] = pattern->cross_bias[k][0];
        bias[i][6] = pattern->cross_bias[k][1];
        for (int j = 0; j < 5; j++) {
          if ((i == k*5) && j < 3) {
            bias[i][j] = bias_row[j+2];
          }
          else if ((i == (k*5)+1) && j < 4) {
            bias[i][j] = bias_row[j+1];

          }
          else if ((i == (k*5)+2) && j < 5) {
            bias[i][j] = bias_row[j];
          }
          else if ((i == (k*5)+3) && j > 0) {
            bias[i][j] = bias_row[j-1];
          }
          else if ((i == (k*5)+4) && j > 1) {
            bias[i][j] = bias_row[j-2];
          }
        }
      }
    }        
  }

 
};

