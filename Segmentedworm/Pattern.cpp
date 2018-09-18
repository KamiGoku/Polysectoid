#include <Oscillator.h>
#include <Pattern.h>

Pattern::Pattern(){
	//body init
	for(int i = 0; i < BOD_FRONT; i++){
		bodyFront[i] = 0;
	}
	for(int i = 0; i < BOD_BACK; i++){
		bodyBack[i] = 0;
	}
	bodyCross[0]=0;
	bodyCross[1]=0;
	//left init
	for(int i = 0; i < LEFT_FRONT; i++){
		leftFront[i] = 0;
	}
	for(int i = 0; i < LEFT_BACK; i++){
		leftBack[i] = 0;
	}
	leftCross[0]=0;
	leftCross[1]=0;
	//right init
	for(int i = 0; i < RIGHT_FRONT; i++){
		rightFront[i] = 0;
	}
	for(int i = 0; i < RIGHT_BACK; i++){
		rightBack[i] = 0;
	}
	rightCross[0]=0;
	rightCross[1]=0;
}

void Pattern::newChainPattern(int location, bool front, double weight[]){
	if(location == 0){//left parapodia
		if(front){//left front
			for(int i = 0; i < LEFT_FRONT; i++){
				leftFront[i]=weight[i];
			}
		}
		else{//left back
			for(int i = 0; i < LEFT_BACK; i++){
				leftBack[i]=weight[i];
			}
		}
	}

	if(location == 1){//body
		if(front){//body front
			for(int i = 0; i < BOD_FRONT; i++){
				bodyFront[i]=weight[i];
			}
		}
		else{//body back
			for(int i = 0; i < BOD_BACK; i++){
				bodyBack[i]=weight[i];
			}
		}
	}

	if(location == 2){//right parapodia
		if(front){//right front
			for(int i = 0; i < RIGHT_FRONT; i++){
				rightFront[i]=weight[i];
			}
		}
		else{//right back
			for(int i = 0; i < RIGHT_BACK; i++){
				rightBack[i]=weight[i];
			}
		}
	}
}

void Pattern::newCrossPattern(int location, double weight[]){
	if(location == 0){//left
		leftCross[0]=weight[0];
		leftCross[1]=weight[1];
	}

	if(location == 1){//body
		bodyCross[0]=weight[0];
		bodyCross[1]=weight[1];
	}

	if(location == 2){//right
		rightCross[0]=weight[0];
		rightCross[1]=weight[1];
	}
}