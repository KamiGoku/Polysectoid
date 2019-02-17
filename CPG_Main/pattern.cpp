#define numBefore 2
#define numAfter 2

class Pattern {

public:
	float bias_pat_before [3][numBefore];
	float bias_pat_after[3][numAfter];

	float weight_pat_before [3][numBefore];
	float weight_pat_after [3][numAfter];

	float cross_weight [3][2];		// 0 is left [body, right], 1 is middle [left,right], 2 is right[left, middle]
	float cross_bias [3][2];

	float int_freq[15];


};