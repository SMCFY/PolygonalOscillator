

#include "Oscillator.h"

Oscillator::Oscillator(int fs, int buffSize)
: pi(MathConstants<float>::pi), f0(60), n(4), t(0.0f), phaseOffset(0.0f), r(0.9f), isClipped(false), pMax(0),
freqRange(Range<int>(60, 2000)), orderRange(Range<int>(3, 30)), teethRange(Range<float>(0.0f, 0.4f)), phaseOffRange(Range<float>(0.0f, MathConstants<float>::twoPi)), radRange(Range<float>(0.1f, 0.9f))
{
    p = new float[buffSize];
    pRender = new float[buffSize];
    polygon = new std::complex<float>[buffSize];
    
    this->fs = fs;
    this->buffSize = buffSize;
    
    theta.reset();

	generatePolygon();
}

Oscillator::~Oscillator()
{
    delete p;
    delete pRender;
	delete polygon;
}

//==============================================================================

void Oscillator::generatePolygon()
{

    dsp::Phase<float> phi;
    isClipped = false;
    pMax = 0;

    for(int i=0; i<buffSize; i++)
    {
        pRender[i] = std::cos(pi/n) / std::cos(fmod(phi.phase+phaseOffset, 2*pi/n) -pi/n + t) * r;
        
        phi.advance(2*pi/buffSize);

        if(pRender[i] > radRange.getEnd()) // checks for clipping (radial amplitude out of range)
        {
            isClipped = true;
            pMax = jmax(pRender[i], pMax); // store maximum clipping radial amplitude
        }
    }

    if(isClipped) // normalize polygon if clipped
    {
        for (int i = 0; i < buffSize; i++)
            pRender[i] = pRender[i]/pMax*radRange.getEnd(); // normalize and cap to range
    }


    phi.reset();
    for(int i=0; i<buffSize; i++) // sampling
    {
        polygon[i].real(pRender[i] * std::cos(phi.phase));
        polygon[i].imag(pRender[i] * std::sin(phi.phase));

        phi.advance(2*pi/buffSize);
    }
}

void Oscillator::synthesizeWaveform(float* buff)
{   

    if(isClipped)
    {
        for(int i = 0; i < buffSize; i++)
        {
            p[i] = std::cos(pi/n) / std::cos(2*pi/n * fmod((theta.phase+phaseOffset)*n/(2*pi), 1) -pi/n + t) * r;  // radial amplitude
    
            buff[i] = p[i]/pMax*radRange.getEnd() * std::sin(theta.phase); // sample the y axis with normalized radial amplitude
    
            theta.advance(2*pi*float(f0)*(1/float(fs)));  // increment phase
        }
    }
    else
    {
        for(int i = 0; i < buffSize; i++)
        {
            p[i] = std::cos(pi/n) / std::cos(2*pi/n * fmod((theta.phase+phaseOffset)*n/(2*pi), 1) -pi/n + t) * r;  // radial amplitude
    
            buff[i] = p[i] * std::sin(theta.phase); // sample the y axis
    
            theta.advance(2*pi*float(f0)*(1/float(fs)));  // increment phase
        }
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

Range<int> Oscillator::getFreqLimits()
{
    return freqRange;
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

int Oscillator::getBufferSize()
{
    return buffSize;
}

Point<float> Oscillator::getDrawCoords(const int& i)
{
	return Point<float>(polygon[i].real(), polygon[i].imag());
}

//==============================================================================

void Oscillator::polyBLAMP()
{
    // aa goes here
}
