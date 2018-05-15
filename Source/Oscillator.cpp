

#include "Oscillator.h"

Oscillator::Oscillator(int fs)
: pi(MathConstants<float>::pi), tableSize(512), tableReadIndex(0), f0(Random().nextInt(Range<int>(110, 880))), n(4), t(0.0f), phaseOffset(0.0f), r(1.0f)
{
	wavetable = new float[tableSize];
    polygon = new std::complex<float>[tableSize];
    
    this->fs = fs;
    tableOverSamplingRatio = float(tableSize)/float(fs);
    tableDelta = f0 * tableOverSamplingRatio;
    
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
        float p = std::cos(pi/n) / std::cos(fmod(theta.phase+phaseOffset, 2*pi/n) - pi/n + t) * r; // radial amplitude
        polygon[i].real(p * cos(theta.phase));
        polygon[i].imag(p * sin(theta.phase));
        
        wavetable[i] = polygon[i].imag(); // projection to wavetable
        
        theta.advance(2*pi/tableSize); // increment phase
	}

}

void Oscillator::synthesizeWaveform(float* buff, const int& buffSize)
{   
    
    for (int sample = 0; sample < buffSize; sample++)
    {
        int i1 = floor(tableReadIndex); // sample index before the readIndex
        int i2; // sample index after the readIndex
        
        if(i1 == tableSize-1)
            i2 = 0; // wrap around sample index
        else
            i2 = i1+1;
        
        float v1 = wavetable[i1];
        float v2 = wavetable[i2];
        
        float frac = tableReadIndex - i1; // sample fraction
        
        buff[sample] = v2 + (frac*(v2-v1)); // linear interpolation to calculate output samples for a single channel
            
            
        tableReadIndex = tableReadIndex + tableDelta; // increment read index
        if(tableReadIndex > tableSize-1)
            tableReadIndex = tableReadIndex-tableSize; // wrap around readIndex if table size is exceeded
    }
    
}

//==============================================================================

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

int Oscillator::getTablesize()
{
	return tableSize;
}

Point<float> Oscillator::getDrawCoords(const int& i)
{

	return Point<float>(polygon[i].real(), polygon[i].imag());
}
