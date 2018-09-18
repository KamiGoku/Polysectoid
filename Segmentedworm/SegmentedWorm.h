#ifndef SEGMENTED_WORM_H
#define SEGMENTED_WORM_H

#include <Oscillator.h>
#include <Pattern.h>

class SegmentedWorm
{
	private:
		Oscillator ganglia[TOTSEG*3];
		/*Put motion patterns here*/
	public:
		SegmentedWorm();
		~SegmentedWorm();
		//SegmentedWorm(double freq[], double phase[], double weight[][]);
		//SegmentedWorm(double freq[], double phase[], double chainWeight[][], double crossWeight[][]);
		/*crossWeight[0 to 5][] diagonal matrix of cross coupling: 
			0.body->left
			1.body->right
			2.left->body
			3.left->right
			4.right->body
			5.right->left
		*/
		void setPhase(double phase[]);
		void setPattern(Pattern input);
		double getPhase(int order);
		double getFreq(int order);
		double getWeight(int inOrder, int outOrder);
};

#endif