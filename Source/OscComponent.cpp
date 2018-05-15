

#include "OscComponent.h"

OscComponent::OscComponent(const Point<float>& p, int fs)
: osc(new Oscillator(fs)), touchHandler(new TouchHandler()),
orderRange(Range<int>(3, 30)), teethRange(Range<float>(0.0f, 1.0f)), phaseOffRange(Range<float>(0.0f, MathConstants<float>::twoPi)), radRange(Range<float>(0.0f, 1.0f)),
compSize(250), lineThickness(5), col(Colour().fromHSV(Random().nextFloat(), 1.0f, 1.0f, 1.0f))
{
	position = p;
	setBounds(position.x-compSize/2, position.y-compSize/2, compSize, compSize);
    size = compSize-50;
    
    drawPoly();
    
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

    g.setColour(col);
    g.strokePath(polyPath, PathStrokeType(lineThickness));
    
}

void OscComponent::drawPoly()
{
    polyPath.clear();
    polyPath.startNewSubPath(mapToScreenCoords(Point<float>(osc->getDrawCoords(0)))); // starting the path at the first point

    for (int i=1; i < osc->getTablesize(); i++)
    {

        polyPath.lineTo(mapToScreenCoords(Point<float>(osc->getDrawCoords(i)))); // add line segment to consequitve points
        
    }

}

Point<float> OscComponent::mapToScreenCoords(const Point<float>& coords)
{

    Point<float> p = coords;
    
    p.x+= 1.0f; // nudge the range to 0 to 2
    p.y+= 1.0f;

    p.x *= size/2; // scale
    p.y *= size/2;

    p.x+= (getWidth()-size)/2; // offset
    p.y+= (getHeight()-size)/2;
    
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

void OscComponent::synthWaveform(float* buff, const int& buffSize)
{
    return osc->synthesizeWaveform(buff, buffSize);
}

//==============================================================================

void OscComponent::timerCallback()
{
	repaint();
}

//==============================================================================

void OscComponent::paint(Graphics& g)
{
    //g.fillAll(Colours::red);
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

        //setBounds(getX()-compSize/2, getY()-compSize/2, compSize*2, compSize*2); // resize component to facilitate interactions
        //drawPoly();
    }
}

void OscComponent::mouseUp(const MouseEvent& e)
{
    touchHandler->rmTouchPoint(e);

    if(touchHandler->getNumPoints() == 0)
    {
        //setBounds(getX()+compSize/2, getY()+compSize/2, compSize, compSize); // reset size
        //drawPoly();
    }
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
            osc->updateRadius(touchHandler->getRadius());
            osc->updateOrder(orderRange.clipValue(abs(touchHandler->getAngle()*20))); // clip the scaled absolute angle to the given range and updtate respective oscilator parameter

            osc->generateWavetable();
            drawPoly(); // re-draw polygon
            break;
        case 3:
            osc->updateTeeth(touchHandler->getRadius());
            osc->updatePhaseOffset(phaseOffRange.clipValue(touchHandler->getAngle()*2));

            osc->generateWavetable();
            drawPoly();
            break;
        default:
            break;
    }
}
