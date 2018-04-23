/*
  ==============================================================================

	Main oscillator class, generating wavetable, and synthesizing a waveform.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <complex>

class Oscillator
{
public:

	Oscillator();
	~Oscillator();

	void generateWavetable();
	void synthesizeWaveform(AudioBuffer<float> buffer);

	void updateParams(const float& r, const float& alpha, const int& numPoints);
	void setSamplingRate(const int& samplingRate);
	float getPolyReal();
	float getPolyImag();

private:

	struct Paramset // core parameter set for the polygonal oscillator
	{
		int f0; // frequency
		float n; // order
		float t; // teeth
		float theta; // phase offset
		float r; // radius (amplitude)
	};

	int fs; // sampling rate
	int tableSize;
	float* wavetable;
    float* p; // radial amplitude of polygon
	std::complex<float>* polygon; // sampled polygon

	float* polyReal; // real part of polygon
	float* polyImag; // imaginary part of polygon

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Oscillator);
};
