/*
  ==============================================================================

	Renders GUI for the oscillators and handles interactions.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Oscillator.h"
#include "TouchHandler.h"


class OscComponent : public Component, public Timer
{
public:

    OscComponent(const Point<float>& p, int fs);
	~OscComponent();

    void setActive();
    void setInactive();
    void markAsActive();
    void synthWaveform(float* buff, const int& buffSize); // calls Oscillator::synthesizeWaveform()
    void renderTouchPoints(Graphics& g); // renders location of touch points and their distances
    
	void timerCallback() override;

	void paint(Graphics& g) override;
    void resized() override;

    void mouseDown(const MouseEvent& e) override;
    void mouseUp(const MouseEvent& e) override;
    void mouseDrag(const MouseEvent& e) override;
    
private:

    void renderPoly(Graphics& g); // renders the geometry
    void drawPoly(); // draws the geometry from line segments
    Point<float> mapToScreenCoords(const Point<float>& coords); // maps the polygon coordinates to the component's screen coordinates

	bool active;
	
	Oscillator* osc;
	ComponentDragger dragger;
	TouchHandler* touchHandler;

	float compSize; // size of the component
	Point<float> position; // component position relative to parent component
	
    float lineThickness; // line thickness of polygon
	float size; // size of rendered polygon
	Colour col;
	Path polyPath; // sequence of line segments to draw the geometry

    float touchIndicatorSize;
    float touchIndicatorThickness;
    float touchIndicatorAlpha;
    Colour touchIndicatorCol;
    float touchIndicatorDash[2] = {15.0f, 10.0f};;

    int f0Ref;
    float orderRef;
    float teethRef;
    float phaseRef;
    float rRef;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscComponent);
};
