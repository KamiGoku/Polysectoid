#include <BasicLinearAlgebra.h>
#include <Oscillator.h>

Oscillator::Oscillator(){
  this->order = 0;
  this->phase = 0;
}

Oscillator::Oscillator(int order, double freq, double phase, double weight[]){
  this->order = order;
  this->phase = phase;
  this->freq = freq;
  for(int i = 0; i < TOTSEG*3 ;i++){
    this->weight[i]=weight[i]; 
  }
}

int Oscillator::getOrder(){
  //get Order
  return order;
}

double Oscillator::getPhase(){
  //get Phase
  return phase;
}

double Oscillator::getFreq(){
  return freq;
}

double Oscillator::getWeight(int input){
  return weight[input];
}

void Oscillator::newPhase(double input){
  phase = input;
}

void Oscillator::newFreq(double input){
  freq = input;
}

void Oscillator::newWeight(int inOrder, double inWeight){
  weight[inOrder]=inWeight;
}


