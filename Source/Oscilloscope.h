/*
  ==============================================================================

    Oscilloscope.h
    Created: 9 Jan 2019 11:04:27am
    Author:  geri

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Oscilloscope : public Component
{
public:
	Oscilloscope(int samplesPerFrame, Colour col, Point<float> pos, float width);
	~Oscilloscope();

	void fillBuffer(float* buff);

	void paint(Graphics& g) override;
    void resized() override;

private:

	int samplesPerFrame;
	Colour col;

	std::vector<Line<float>> waveformSegmentBuffer;
	std::vector<Line<float>> waveformSegmentRender;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Oscilloscope);
};
