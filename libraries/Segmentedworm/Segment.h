#ifndef SEGMENT_H
#define SEGMENT_H

#include <BasicLinearAlgebra.h>
#include <Oscillator.h>

class Segment
{
private:
	Oscillator OSC[3];
	int order;
public:
	Segment();
	~Segment();
	
};

#endif