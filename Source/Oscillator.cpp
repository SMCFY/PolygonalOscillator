

#include "Oscillator.h"

Oscillator::Oscillator()
: tableSize(512)
{
	
}

Oscillator::~Oscillator()
{
	//delete waveTable;
	//delete polygon;
}

//==============================================================================

void Oscillator::generateWavetable()
{

}

void Oscillator::synthesizeWaveform(AudioBuffer<float> buffer)
{

}

//==============================================================================

void Oscillator::updateParams(const float& r, const float& alpha, const int& numPoints)
{

}

void Oscillator::setTableSize(const int& size)
{
	tableSize = size;
}

void Oscillator::setSamplingRate(const int& samplingRate)
{
	fs = samplingRate;
}
