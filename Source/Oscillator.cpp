

#include "Oscillator.h"

Oscillator::Oscillator()
: pi(MathConstants<float>::pi), tableSize(512), f0(440), n(4), t(0), phaseOffset(0.0f), r(1.0f)
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
    
    //dsp::Phase<float> theta;
    float theta = 2*pi/tableSize;

	for(int i=0; 1<tableSize; i++)
	{
        float p = std::cos(pi/n) / std::cos(fmod(theta*i, 2*pi/n) - pi/n + t); // radial amplitude
        //polygon[i] = p * (std::cos(theta*i+phaseOffset) + 1j*std::sin(theta*i+phaseOffset)); // sampled geometry
        polygon[i].real(p * cos(theta*i+phaseOffset));
        polygon[i].imag(p * sin(theta*i+phaseOffset));
        
        wavetable[i] = polygon[i].imag(); // projection to wavetable
        
        //theta.advance(2*pi/tableSize); // increment phase
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

OwnedArray<Point<float>> Oscillator::getDrawCoords()
{

	OwnedArray<Point<float>> drawCoords;

	for (int i = 0; i < tableSize; i++)
    {
        Point<float> p = Point<float>(polygon[i].real(), polygon[i].imag());
		drawCoords.add(&p);
    }

	return drawCoords;
}
