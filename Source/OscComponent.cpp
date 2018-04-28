

#include "OscComponent.h"

OscComponent::OscComponent(const Point<float>& p)
: osc(new Oscillator()), touchHandler(new TouchHandler()),
compSize(150), lineThickness(5), col(Colour().fromHSV(Random().nextFloat(), 1.0f, 1.0f, 1.0f))
{
	position = p;
	setBounds(position.x-compSize/2, position.y-compSize/2, compSize, compSize);
    size = compSize-lineThickness;
    
    startTimerHz(30);
}


OscComponent::~OscComponent()
{
	delete touchHandler;
	delete osc;
}

//==============================================================================

void OscComponent::renderPoly(Graphics& g)
{

    Path polyPath;
    polyPath.startNewSubPath(mapToScreenCoords(Point<float>(osc->getDrawCoords(0)))); // starting the path at the first point

    for (int i=1; i < osc->getTablesize(); i++)
    {

        polyPath.lineTo(mapToScreenCoords(Point<float>(osc->getDrawCoords(i)))); // add line segment to consequitve points
        
    }

    g.setColour(col);
    // g.drawEllipse((getWidth()-size)/2, (getWidth()-size)/2, size, size, lineThickness);
    g.strokePath(polyPath, PathStrokeType(lineThickness));
    
}

Point<float> OscComponent::mapToScreenCoords(const Point<float>& coords)
{

    Point<float> p = coords;
    
    p.x+= 1.0f; // nudge the range to 0 to 2
    p.y+= 1.0f;

    p.x *= size/2; // scale
    p.y *= size/2;
    
    return p;
}

void OscComponent::setActive()
{
	active = true;
    col = col.withSaturation(1.0f);
    col = col.withBrightness(1.0f);
}

void OscComponent::setInactive()
{
    active = false;
    col = col.withSaturation(0.4f);
    col = col.withBrightness(0.4f);
}

void OscComponent::markAsActive()
{
	toFront(true);
}


//==============================================================================

void OscComponent::timerCallback()
{
	repaint();
}

//==============================================================================

void OscComponent::paint(Graphics& g)
{
    renderPoly(g);
}

void OscComponent::resized()
{

}

//==============================================================================

void OscComponent::mouseDown(const MouseEvent& e)
{
    touchHandler->addTouchPoint(e);
    
    if(touchHandler->getNumPoints() == 1) // selection and dragging of oscillators
    {
        markAsActive();
        dragger.startDraggingComponent(this, e);
    }
	
}

void OscComponent::mouseUp(const MouseEvent& e)
{
    touchHandler->rmTouchPoint(e);
}

void OscComponent::mouseDrag(const MouseEvent& e)
{
    touchHandler->updatePoints(e);
    
    switch(touchHandler->getNumPoints()) // mapping based on number of touch points
    {
        case 1:
            dragger.dragComponent(this, e, nullptr);
            break;
        case 2:
            size = 145 * touchHandler->getRadius();
            lineThickness = round(touchHandler->getAngle()*10);
            break;
        case 3:
            setAlpha(touchHandler->getRadius());
            touchHandler->getAngle();
            break;
        default:
            break;
    }
}
