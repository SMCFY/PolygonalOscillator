

#include "Oscillator.h"

Oscillator::Oscillator()
: tableSize(512)
{
	polyReal = new float[tableSize];
	polyImag = new float[tableSize];
}

Oscillator::~Oscillator()
{
	delete polyReal;
	delete polyImag;
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

	//polyReal = polygon->real();
	//polyImag = polygon->imag();
}

void Oscillator::setSamplingRate(const int& samplingRate)
{
	fs = samplingRate;
}

float Oscillator::getPolyReal()
{
	return *polyReal;
}

float Oscillator::getPolyImag()
{
	return *polyImag;
}
