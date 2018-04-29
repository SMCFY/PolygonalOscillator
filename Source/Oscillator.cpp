

#include "Oscillator.h"

Oscillator::Oscillator()
: pi(MathConstants<float>::pi), tableSize(512), f0(440), n(4), t(0.0f), phaseOffset(0.0f), r(1.0f)
{

	wavetable = new float[tableSize];
    polygon = new std::complex<float>[tableSize];

	generateWavetable();
}

Oscillator::~Oscillator()
{
	delete wavetable;
	delete polygon;
}

//==============================================================================

void Oscillator::generateWavetable()
{
    
    dsp::Phase<float> theta;

	for(int i=0; i<tableSize; i++)
	{
        float p = std::cos(pi/n) / std::cos(fmod(theta.phase, 2*pi/n) - pi/n + t) * r; // radial amplitude
        polygon[i].real(p * cos(theta.phase+phaseOffset));
        polygon[i].imag(p * sin(theta.phase+phaseOffset));
        
        wavetable[i] = polygon[i].imag(); // projection to wavetable
        
        theta.advance(2*pi/tableSize); // increment phase
	}
}

void Oscillator::synthesizeWaveform(AudioBuffer<float> buffer)
{

}

//==============================================================================

//void Oscillator::updateParams(const int& f0, const float& n, const float& t, const float& phaseOffset, const float& r)
//{
//	this->f0 = f0;
//	this->n = n;
//	this->t = t;
//	this->phaseOffset = phaseOffset;
//	this->r = r;
//
//
//	generateWavetable();
//}

void Oscillator::updateFreq(const int& f0)
{
	this->f0 = f0;
}

void Oscillator::updateOrder(const float& n)
{
	this->n = n;
}

void Oscillator::updateTeeth(const float& t)
{
	this->t = t;
}

void Oscillator::updatePhaseOffset(const float& phaseOffset)
{
	this->phaseOffset = phaseOffset;
}

void Oscillator::updateRadius(const float& r)
{	
	this->r = r;
}

void Oscillator::setSamplingRate(const int& samplingRate)
{
	fs = samplingRate;
}

int Oscillator::getTablesize()
{
	return tableSize;
}

Point<float> Oscillator::getDrawCoords(const int& i)
{

	return Point<float>(polygon[i].real(), polygon[i].imag());
}
