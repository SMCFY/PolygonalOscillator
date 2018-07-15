

#include "TouchHandler.h"

TouchHandler::TouchHandler()
: rMax(300), rMin(60), areaMax(40000), areaMin(4000)
{
	
}

TouchHandler::~TouchHandler()
{
    arrayOfTouchPoints.clear(true);
}

//==============================================================================

void TouchHandler::addTouchPoint(const MouseEvent& e)
{
    TouchPoint* tp = new TouchPoint(e.source);
    arrayOfTouchPoints.add(tp);

    // update reference parameters
    if(getNumPoints() == 2) // anchor
    {
        radRef = getNormalizedDist();
        alphaRef = getNormalizedAngle();
    }
    else if(getNumPoints() == 3) // polygon
    {
        areaRef = getNormalizedArea();
        orientationRef = 0;
    }
}

void TouchHandler::rmTouchPoint(const MouseEvent& e)
{
    for (int i=0; i < getNumPoints(); i++)
    {
    	if(arrayOfTouchPoints[i]->sourceIndex == e.source.getIndex())
    		arrayOfTouchPoints.removeObject(arrayOfTouchPoints[i]);
    }
    
    // update reference parameters
    if(getNumPoints() == 2) // anchor
    {
        radRef = getNormalizedDist();
        alphaRef = getNormalizedAngle();
    }
    else if(getNumPoints() == 3) // polygon
    {
        areaRef = getNormalizedArea();
        orientationRef = 0;
    }
}

int TouchHandler::getNumPoints()
{
    return arrayOfTouchPoints.size();
}

void TouchHandler::updatePoints(const MouseEvent& e)
{
    for (int i=0; i < getNumPoints(); i++)
    {
        if(arrayOfTouchPoints[i]->sourceIndex == e.source.getIndex())
            arrayOfTouchPoints[i]->pos = e.source.getScreenPosition();
    }
}

Point<float> TouchHandler::getTouchPos(const int& i)
{
    return arrayOfTouchPoints[i]->pos;
}

//==============================================================================

float TouchHandler::getAnchorRadiusDelta()
{
    return getNormalizedDist() - radRef;
}

float TouchHandler::getAnchorAngleDelta()
{
    return getNormalizedAngle() - alphaRef;
}

float TouchHandler::getTriAreaDelta()
{
    return getNormalizedArea() - areaRef;
}

float TouchHandler::getTriOrientationDelta()
{
    return getNormalizedOrientation();
}

//==============================================================================

float TouchHandler::getNormalizedDist()
{
    float dist = std::sqrt(std::pow(arrayOfTouchPoints[0]->pos.x-arrayOfTouchPoints[arrayOfTouchPoints.size()-1]->pos.x,2) + std::pow(arrayOfTouchPoints[0]->pos.y-arrayOfTouchPoints[arrayOfTouchPoints.size()-1]->pos.y,2));
    return jmin(jmax(dist-rMin, 0.0f), rMax) / rMax; // normalized, capped distance
}

float TouchHandler::getNormalizedAngle()
{
	float x = arrayOfTouchPoints[arrayOfTouchPoints.size()-1]->pos.x-arrayOfTouchPoints[0]->pos.x;
	float y = arrayOfTouchPoints[arrayOfTouchPoints.size()-1]->pos.y-arrayOfTouchPoints[0]->pos.y;
    return atan2(y,x) / MathConstants<float>::pi;

}

float TouchHandler::getNormalizedArea()
{
    // coordinates of the triangle
    Point<float> a = arrayOfTouchPoints[0]->pos; Point<float> b = arrayOfTouchPoints[1]->pos; Point<float> c = arrayOfTouchPoints[2]->pos;

    float area = abs((a.x*(b.y-c.y) + b.x*(c.y-a.y) + c.x*(a.y-b.y)) / 2);
    return jmin(jmax(area-areaMin, 0.0f), areaMax) / areaMax; // normalized, capped area
}

float TouchHandler::getNormalizedOrientation()
{
    return 0;
    
}
