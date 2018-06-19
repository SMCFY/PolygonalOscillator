

#include "TouchHandler.h"

TouchHandler::TouchHandler()
: rMax(300.0f), rMin(60.0f)
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

    if(getNumPoints() > 1) // update reference parameters
    {
        radRef = getAbsNormalizedDist();
        alphaRef = getNormalizedAngle();
    }
}

void TouchHandler::rmTouchPoint(const MouseEvent& e)
{
    for (int i=0; i < getNumPoints(); i++)
    {
    	if(arrayOfTouchPoints[i]->sourceIndex == e.source.getIndex())
    		arrayOfTouchPoints.removeObject(arrayOfTouchPoints[i]);
    }

    if(getNumPoints() > 1) // update reference parameters
    {
        radRef = getAbsNormalizedDist();
        alphaRef = getNormalizedAngle();
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

//==============================================================================

float TouchHandler::getRadiusDelta()
{
    return getAbsNormalizedDist() - radRef;
}

float TouchHandler::getAngleDelta()
{
    return getNormalizedAngle() - alphaRef;
}

//==============================================================================

float TouchHandler::getAbsNormalizedDist()
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
