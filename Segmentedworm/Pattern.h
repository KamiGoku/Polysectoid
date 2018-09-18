#ifndef PATTERN_H
#define PATTERN_H

#include <Oscillator.h>

/*Less than TOTSEG*/
const int BOD_FRONT = 3;
const int BOD_BACK = 2;
const int LEFT_FRONT = 3;
const int LEFT_BACK = 2;
const int RIGHT_FRONT = 3;
const int RIGHT_BACK = 2;


class Pattern
{
	private:
		//body properties
		double bodyFront[BOD_FRONT];
		double bodyBack[BOD_BACK];
		double bodyCross[2];//0:left;1:right
		//left parapodia property
		double leftFront[LEFT_FRONT];
		double leftBack[LEFT_BACK];
		double leftCross[2];//0:body;1:right
		//right parapodia property
		double rightFront[RIGHT_FRONT];
		double rightBack[RIGHT_BACK];
		double rightCross[2];//0:left;1:body
	public:
		Pattern();
		~Pattern();
		//back:False,front:True:0,body:1,right:2//
		void newChainPattern(int location, bool front, double weight[]);
		void newCrossPattern(int location, double weight[]);
		double getChainweight(int location, bool front, int order);
		double getCrossweight(int location, int oppose);
};

#endif