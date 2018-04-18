/*
  ==============================================================================

	Renders GUI for the oscillators.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class OscComponent : private AnimatedAppComponent
{
public:

	OscComponent(const Point<float>& p);
	~OscComponent();

	void setPoly(const float& poly);

	void update() override; 

	void timerCallback() override;

	void paint(Graphics& g) override;
    void resized() override;

    // void mouseDown(const MouseEvent& e) override;
    // void mouseDrag(const MouseEvent& e) override;
    // void mouseUp(const MouseEvent& e) override;
    // void mouseDoubleClick(const MouseEvent &event) override;
    // void mouseMagnify(const MouseEvent &event, float scaleFactor) override;



private:

	//int* const id; // ID
	const int fps; // refresh rate
	float compSize; // size of the component
	Point<float> position;
	float* polygon; // geometry to draw
	Colour colour; 

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscComponent);
};
