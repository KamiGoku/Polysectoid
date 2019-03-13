#include <Oscillator.h>
#include <SegmentedWorm.h>
#include <Pattern.h>

SegmentedWorm::SegmentedWorm(){
	/*for(int i=0; i < TOTSEG*3;i++){
		ganglia[i].Oscillator();
	}*/
}
void SegmentedWorm::setPhase(double phase[]){
	for(int i=0; i < TOTSEG*3;i++){
		ganglia[i].newPhase(phase[i]);
	}
}
double SegmentedWorm::getPhase(int order){
	return((double) ganglia[order].getPhase());
}
double SegmentedWorm::getFreq(int order){
	return((double) ganglia[order].getFreq());
}
double SegmentedWorm::getWeight(int inOrder, int outOrder){
	return((double) ganglia[inOrder].getWeight(outOrder));
}
void SegmentedWorm::setPattern(Pattern input){
	
}