/*
  ==============================================================================

	Stores individual touch points, and provides meaningful parameters from raw touch data.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class TouchHandler
{
public:

	TouchHandler();
	~TouchHandler();

	void addTouchPoint(const MouseEvent& e, const int& id);
	void rmTouchPoint(const MouseEvent& e);
	void getNumPoints(); // return the total number of touch points on the screen


	float getRadius(const int& numberOfPoints);
	float getAngle(const int& numberOfPoints);
	float getRadiusChange();
	float getAngleChange();

private:

	float r2; // anchor radius 2 finger
	float alpha2; // anchor angle 2 fingers
	float r3; // anchor radius 3 fingers
	float alpha3; // anchor angle 3 fingers

	typedef struct TouchPoint
	{
		int const* oscId; // ID matching the closest oscillator instance
		Point<float> pos; // position
		int sourceIndex; // MouseInputSource index
		TouchPoint(const MouseInputSource& mouseInput, const int& id)
        {
        	oscId = &id;
			pos = mouseInput.getScreenPosition();
			sourceIndex = mouseInput.getIndex();
        }
	} TouchPoint;

	OwnedArray<TouchPoint> arrayOfTouchPoints;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TouchHandler);
	
};