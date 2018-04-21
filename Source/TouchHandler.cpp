

#include "TouchHandler.h"

TouchHandler::TouchHandler()
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
}

void TouchHandler::rmTouchPoint(const MouseEvent& e)
{
    for (int i=0; i < arrayOfTouchPoints.size(); i++)
    {
    	if(arrayOfTouchPoints[i]->sourceIndex == e.source.getIndex())
    		arrayOfTouchPoints.removeObject(arrayOfTouchPoints[i]);
    }
}

int TouchHandler::getNumPoints()
{
    return arrayOfTouchPoints.size();
}

//==============================================================================

float TouchHandler::getRadius()
{
    if(getNumPoints() == 2)
        return r2;
    else if(getNumPoints() == 3)
        return r3;
    else
        return NULL;
}

float TouchHandler::getAngle()
{
    if(getNumPoints() == 2)
        return alpha2;
    else if(getNumPoints() == 3)
        return alpha3;
    else
        return NULL;
}

float TouchHandler::getRadiusChange()
{
    if(getNumPoints() == 2)
        return r2delta;
    if(getNumPoints() == 3)
        return r3delta;
    else
        return NULL;
}

float TouchHandler::getAngleChange()
{
    if(getNumPoints() == 2)
        return alpha2delta;
    if(getNumPoints() == 3)
        return alpha3delta;
    else
        return NULL;
}
