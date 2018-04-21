/*
  ==============================================================================

	Renders GUI for the oscillators.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Oscillator.h"
#include "TouchHandler.h"


class OscComponent : public Component, public Timer
{
public:

    OscComponent(const Point<float>& p);
	~OscComponent();

	void setPoly(const float& poly);
    void renderPoly(Graphics& g);
    void setActive();
    void setInactive();
    void markAsActive();

	void timerCallback() override;

	void paint(Graphics& g) override;
    void resized() override;

    void mouseDown(const MouseEvent& e) override;
    void mouseDrag(const MouseEvent& e) override;
    void mouseUp(const MouseEvent& e) override;
    
    
private:

	bool active;
	
	Oscillator* osc;
	ComponentDragger dragger;
	TouchHandler* touchHandler;

	float compSize; // size of the component
	Point<float> position; // component position relative to parent component
    float lineThickness;
	float size; // size of rendered polygon
	Colour col;

	float* polygon; // geometry to draw
	
	

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscComponent);
};
