#define numBefore 2
#define numAfter 2
#define NUM 5
#define SIZE NUM + 2

// Brain Variables  
/*const float bias_pat_before[3][numBefore] = {{.20, .20},
                                             {.20, .20},
                                             {.20, .20}};
const float bias_pat_after[3][numAfter] = {{.20, .20},
                                           {.20, .20},
                                           {.20, .20}};*/
const float bias_pat_before[3][numBefore] = {{.10, .10},
                                             {.10, .10},
                                             {.10, .10}};
const float bias_pat_after[3][numAfter] = {{.10, .10},
                                           {.10, .10},
                                           {.10, .10}};                                           

const float weight_pat_before [3][numBefore] = {{0.008, 0.004},
                                                {0.02, 0.01},
                                                {0.008, 0.004}};
const float weight_pat_after [3][numAfter] = {{0.008, 0.004},
                                              {0.02, 0.01},
                                              {0.008, 0.004}};

const float cross_weight [3][2] = {{0.0003, 0.0003},
                                   {0.0003, 0.0003},
                                   {0.0003, 0.0003}};
                             
const float cross_bias [3][2] = {{0, 0},
                                 {0, 0},
                                 {0, 0}};

const float int_freq[15] = {0.02, 0.02, 0.02, 0.02, 0.02, 0.01, 0.01, 0.01, 0.01, 0.01, 0.02, 0.02, 0.02, 0.02, 0.02};

const float init_phases [15] = {.1, .7, .8, .23, .6, .42, .7, .9, .8, .6, .73, .7, .5, .44, .6};

const float tau = 3.2;                                // Frequency parameter


class Brain {
public:
	float weights_bias[NUM*3][SIZE] = {{0}};	      // Integer part = weight, Fractional part = bias

	Brain() {
    for (int k = 0; k < 3; k++) {
      float weight_row[NUM];
      float bias_row[NUM];
      
      bias_row[0] = bias_pat_after[k][0] + bias_pat_after[k][1];  // Assume negative
      bias_row[1] = bias_pat_after[k][0];                         // Assume negative
      bias_row[2] = 0;
      bias_row[3] = bias_pat_before[k][0];
      bias_row[4] = bias_pat_before[k][0] + bias_pat_before[k][1];
      
      weight_row[0] = weight_pat_after[k][1];
      weight_row[1] = weight_pat_after[k][0];
      weight_row[2] = 0;
      weight_row[3] = weight_pat_before[k][0];
      weight_row[4] = weight_pat_before[k][1];  

      for (int i = k*NUM; i < (k+1)*NUM; i++) {
        weights_bias[i][5] += cross_weight[k][0]*10000;
        weights_bias[i][6] += cross_weight[k][1]*10000;
        weights_bias[i][5] += cross_bias[k][0];
        weights_bias[i][6] += cross_bias[k][1];
        
        for (int j = 0; j < 5; j++) {
          if ((i == k*5) && j < 3) {
            weights_bias[i][j] += weight_row[j + 2]*10000;
            weights_bias[i][j] += bias_row[j + 2];
          }
          else if ((i == (k*5)+1) && j < 4) {
            weights_bias[i][j] += weight_row[j + 1]*10000;
            weights_bias[i][j] += bias_row[j + 1];
          }
          else if ((i == (k*5)+2) && j < 5) {
            weights_bias[i][j] += weight_row[j]*10000;
            weights_bias[i][j] += bias_row[j];
          }
          else if ((i == (k*5)+3) && j > 0) {
            weights_bias[i][j] += weight_row[j - 1]*10000;
            weights_bias[i][j] += bias_row[j - 1];
          }
          else if ((i == (k*5)+4) && j > 1) {
            weights_bias[i][j] += weight_row[j - 2]*10000;
            weights_bias[i][j] += bias_row[j - 2];
          }
        }
      }
    }    
	} 
};
