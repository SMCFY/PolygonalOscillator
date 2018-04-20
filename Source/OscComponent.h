/*
  ==============================================================================

	Renders GUI for the oscillators.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class OscComponent : public Component, public Timer
{
public:

	OscComponent(const Point<float>& p);
	~OscComponent();

	void setPoly(const float& poly);
    void renderPoly(Graphics& g);
    void setActive(const bool& a);
    bool isActive();

	void timerCallback() override;

	void paint(Graphics& g) override;
    void resized() override;

    // void mouseDown(const MouseEvent& e) override;
    // void mouseDrag(const MouseEvent& e) override;
    // void mouseUp(const MouseEvent& e) override;
    // void mouseDoubleClick(const MouseEvent &event) override;
    // void mouseMagnify(const MouseEvent &event, float scaleFactor) override;



private:

	bool active; 
	float compSize; // size of the component
	Point<float> position; // component position relative to parent component
	float* polygon; // geometry to draw
	Colour col; 

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscComponent);
};
