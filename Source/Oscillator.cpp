

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
        float p = std::cos(pi/n) / std::cos(fmod(theta.phase, 2*pi/n) - pi/n + t); // radial amplitude
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

void Oscillator::updateParams(const float& r, const float& alpha, const int& numPoints)
{

	generateWavetable();
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
