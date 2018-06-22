

#include "Oscillator.h"

Oscillator::Oscillator(int fs)
: pi(MathConstants<float>::pi), tableSize(512), tableReadIndex(0), f0(Random().nextInt(Range<int>(110, 880))), n(4), t(0.0f), phaseOffset(0.0f), r(0.9f),
freqRange(Range<int>(60, 2000)), orderRange(Range<int>(3, 30)), teethRange(Range<float>(0.0f, 0.4f)), phaseOffRange(Range<float>(0.0f, MathConstants<float>::twoPi)), radRange(Range<float>(0.1f, 0.9f))
{
    p = new float[tableSize];
	wavetable = new float[tableSize];
    polygon = new std::complex<float>[tableSize];
    
    this->fs = fs;
    tableOverSamplingRatio = float(tableSize)/float(fs);
    tableDelta = f0 * tableOverSamplingRatio;
    
	generateWavetable();
}

Oscillator::~Oscillator()
{
    delete p;
	delete wavetable;
	delete polygon;
}

//==============================================================================

void Oscillator::generateWavetable()
{
    bool isClipped = false;
    float pMax = 0; // maximum radial amplitude
    
    dsp::Phase<float> theta;
	for(int i=0; i<tableSize; i++)
	{
        p[i] = std::cos(pi/n) / std::cos(fmod(theta.phase+phaseOffset, 2*pi/n) - pi/n + t) * r; // radial amplitude

        if(p[i] > radRange.getEnd()) // checks for clipping (radial amplitude out of range)
        {
            isClipped = true;
            pMax = jmax(p[i], pMax); // store maximum clipping radial amplitude
        }

        theta.advance(2*pi/tableSize); // increment phase
    }

    if(isClipped) // normalize polygon if clipped
    {
        for (int i = 0; i < tableSize; i++)
            p[i] = p[i]/pMax*radRange.getEnd(); // normalize and cap to range
    }

    theta.reset(); // reset phase
    for(int i=0; i<tableSize; i++)
    {
        // sample polygon
        polygon[i].real(p[i] * cos(theta.phase));
        polygon[i].imag(p[i] * sin(theta.phase));

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
	this->f0 = freqRange.clipValue(f0);
}

void Oscillator::updateOrder(const int& n)
{
	this->n = orderRange.clipValue(n);
}

void Oscillator::updateTeeth(const float& t)
{
	this->t = teethRange.clipValue(t);
}

void Oscillator::updatePhaseOffset(const float& phaseOffset)
{
	this->phaseOffset = phaseOffRange.clipValue(phaseOffset);
}

void Oscillator::updateRadius(const float& r)
{	
	this->r = radRange.clipValue(r);
}

//==============================================================================

int Oscillator::getFreq()
{
    return f0;
}

int Oscillator::getOrder()
{
    return n;
}

float Oscillator::getTeeth()
{
    return t;
}

float Oscillator::getPhaseOffset()
{
    return phaseOffset;
}

float Oscillator::getRadius()
{
    return r;
}

int Oscillator::getTablesize()
{
	return tableSize;
}

Point<float> Oscillator::getDrawCoords(const int& i)
{
	return Point<float>(polygon[i].real(), polygon[i].imag());
}
