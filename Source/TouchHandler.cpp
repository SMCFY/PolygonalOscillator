

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
}

void TouchHandler::rmTouchPoint(const MouseEvent& e)
{
    for (int i=0; i < getNumPoints(); i++)
    {
    	if(arrayOfTouchPoints[i]->sourceIndex == e.source.getIndex())
    		arrayOfTouchPoints.removeObject(arrayOfTouchPoints[i]);
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

float TouchHandler::getRadius()
{
    if(getNumPoints() == 2)
    {
        r2 = getAbsNormalizedDist();
        return r2;
    }
    else if(getNumPoints() == 3)
    {
        r3 = getAbsNormalizedDist();
        return r3;
    }
    else
        return NULL;
}

float TouchHandler::getAngle()
{
//    if(getNumPoints() == 2)
//        return alpha2;
//    else if(getNumPoints() == 3)
//        return alpha3;
//    else
//        return NULL;
}

float TouchHandler::getRadiusChange()
{
//    if(getNumPoints() == 2)
//        return r2delta;
//    else if(getNumPoints() == 3)
//        return r3delta;
//    else
//        return NULL;
}

float TouchHandler::getAngleChange()
{
//    if(getNumPoints() == 2)
//        return alpha2delta;
//    else if(getNumPoints() == 3)
//        return alpha3delta;
//    else
//        return NULL;
}

//==============================================================================

float TouchHandler::getAbsNormalizedDist()
{
    float dist = std::sqrt(std::pow(arrayOfTouchPoints[0]->pos.x-arrayOfTouchPoints[1]->pos.x,2) + std::pow(arrayOfTouchPoints[0]->pos.y-arrayOfTouchPoints[1]->pos.y,2));
    return jmin(jmax(dist-rMin, 0.0f), rMax) / rMax; //normalized, capped distance
}
