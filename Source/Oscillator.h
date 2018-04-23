/*
  ==============================================================================

	Main oscillator class, generating wavetable, and synthesizing a waveform.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Oscillator
{
public:

	Oscillator();
	~Oscillator();

	void generateWavetable();
	void synthesizeWaveform(AudioBuffer<float> buffer);

	void updateParams(const float& r, const float& alpha, const int& numPoints);
	void setTableSize(const int& size);
	void setSamplingRate(const int& samplingRate);

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
	float* polygon; // radial amplitude of geometry

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Oscillator);
};
