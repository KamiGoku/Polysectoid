#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include <BasicLinearAlgebra.h>

const static int TOTSEG = 10;//Total number of segment
const static double pi = 3.14159;//define pi

class Oscillator 
{
  private:
    int order; //order of the oscillator
    //[ 0 to (TOTSEG-1):BODY; TOTSEG to (2*TOTSEG-1): LEFT parapodia; (2*TOTSEG) to (3*TOTSEG-1): RIGHT parapodia]
    double freq;
    double phase; //phase of an oscillator 0<=phase<2*pi
    double weight[TOTSEG*3];
  public:
    Oscillator();
    //~Oscillator();
    Oscillator(int order, double freq, double phase, double weight[]);
    int getOrder(); //get Order
    double getPhase(); //get Phase
    double getFreq();
    double getWeight(int input);
    void newPhase(double input);
    void newFreq(double input);
    void newWeight(int inOrder, double inWeight);
};

#endif
