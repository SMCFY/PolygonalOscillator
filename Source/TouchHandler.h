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
    Point<float> getTouchPos(const int& i); // returns screen position for indexed touch point

	float getAnchorRadiusDelta();
	float getAnchorAngleDelta();
	float getTriAreaDelta();
	float getTriRotationDelta();

private:

    float rMax; // maximum anchor radius
    float rMin; // minimum anchor radius
    float areaMax; // maximum triangle area
    float areaMin; // minimum triangle area

    float radRef; // reference for incremental change
    float alphaRef;
    float areaRef;
    float rotationRef;
    Point<float> originRef; // the triangle's centroid defined by 3 touch points

    float getNormalizedDist(); // calculates the normalized absolute distance between 2 touch points
    float getNormalizedAngle(); // calculates the normalized angle between 2 touch points
    float getNormalizedArea(); // calculates the area of a triangle defined by 3 touch points
    float getNormalizedRotation(); // calculates the rotation of a triangle defined by 3 touch points

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
