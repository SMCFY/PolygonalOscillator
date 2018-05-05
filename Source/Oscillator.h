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

	Oscillator(int fs);
	~Oscillator();

	void generateWavetable();
	float** synthesizeWaveform(const int& numberOfChannels, const int& samplesPerFrame);

	//void updateParams(const int& f0, const float& n, const float& t, const float& phaseOffset, const float& r);
	void updateFreq(const int& f0);
	void updateOrder(const float& n);
	void updateTeeth(const float& t);
	void updatePhaseOffset(const float& phaseOffset);
	void updateRadius(const float& r);
	int getTablesize();
	Point<float> getDrawCoords(const int& i); // returns the cartesian coordinates of the sampled geometry for rendering

private:

    int f0; // frequency
    float n; // order
    float t; // teeth
    float phaseOffset; // phase offset in radians
    float r; // radius (amplitude)

	int fs; // sampling rate
	int tableSize;
    float pi;

    float tableOverSamplingRatio; // oversampling ratio for synthesis
    float tableDelta; // read increment for wavetable
    float tableReadIndex; // init table read index

	float* wavetable;
	std::complex<float>* polygon; // sampled polygon

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Oscillator);
};
