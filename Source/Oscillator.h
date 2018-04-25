/*
  ==============================================================================

	Main oscillator class, generating wavetable, and synthesizing a waveform.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <complex>
#include "math.h"


class Oscillator
{
public:

	Oscillator();
	~Oscillator();

	void generateWavetable();
	void synthesizeWaveform(AudioBuffer<float> buffer);

	void updateParams(const float& r, const float& alpha, const int& numPoints);
	void setSamplingRate(const int& samplingRate);
	OwnedArray<Point<float>> getDrawCoords(); // returns the cartesian coordinates of the sampled geometry for rendering

private:

    int f0; // frequency
    float n; // order
    float t; // teeth
    float phaseOffset; // phase offset in radians
    float r; // radius (amplitude)

	int fs; // sampling rate
	int tableSize;
    float pi;
    
	float* wavetable;
    //float* p; // radial amplitude of polygon
	std::complex<float>* polygon; // sampled polygon
	//float* polyReal; // real part of polygon
	//float* polyImag; // imaginary part of polygon

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Oscillator);
};
