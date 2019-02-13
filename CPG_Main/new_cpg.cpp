#include <math.h>

double Test_const = 0.1;
double Limb_const = 0.0003;
double Left_const = 0.004;
double Right_const = 0.004;

class Variables {
public: 
	double weights [15][7];
	double intrinsic [15];
	double bias [15][7] ;
	double tau;

	Variables() {
		for (int i = 6; i < 8; i++) {
			for (int j = 0; j < 7; j++) {
				weights[i][j] = 1;
			}
		}



		tau = 0;


	}


};

int main() {
	Variables *var = new Variables();
	






}
