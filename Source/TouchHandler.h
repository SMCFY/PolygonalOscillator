/*
  ==============================================================================

	Stores individual touch points, and provides parameters from raw touch data.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "math.h"

class TouchHandler
{
public:

	TouchHandler();
	~TouchHandler();

	void addTouchPoint(const MouseEvent& e);
	void rmTouchPoint(const MouseEvent& e);
	int getNumPoints(); // return the total number of touch points on the screen
    void updatePoints(const MouseEvent& e); // update the location of touch points

	float getRadius();
	float getAngle();
	float getRadiusChange();
	float getAngleChange();

private:

    float rMax; // maximum anchor radius
    float rMin; // minimum anchor radius
    
    float r2; // anchor radius 2 finger
    float alpha2; // anchor angle 2 fingers
    float r3; // anchor radius 3 fingers
    float alpha3; // anchor angle 3 fingers

//    float r2delta; // anchor radius increment 2 finger
//    float alpha2delta; // anchor angle increment 2 finger
//    float r3delta;
//    float alpha3delta;

    float getAbsNormalizedDist(); // calculates the normalized absolute distance between 2 touch points
    float getNormalizedAngle(); // calculates the normalized angle between 2 touch points
    
	typedef struct TouchPoint
	{
		Point<float> pos; // position
		int sourceIndex; // MouseInputSource index
		TouchPoint(const MouseInputSource& mouseInput)
        {
			pos = mouseInput.getScreenPosition();
			sourceIndex = mouseInput.getIndex();
        }
	} TouchPoint;

	OwnedArray<TouchPoint> arrayOfTouchPoints;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TouchHandler);
	
};
