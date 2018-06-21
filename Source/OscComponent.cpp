

#include "OscComponent.h"

OscComponent::OscComponent(const Point<float>& p, int fs)
: osc(new Oscillator(fs)), touchHandler(new TouchHandler()),
touchIndicatorSize(50), touchIndicatorThickness(1), touchIndicatorAlpha(0.3f), touchIndicatorCol(Colours::white),
compSize(300), lineThickness(5), col(Colour().fromHSV(Random().nextFloat(), 1.0f, 1.0f, 1.0f))
{
	position = p;
	setBounds(position.x-compSize/2, position.y-compSize/2, compSize, compSize);
    size = compSize-50;
    
    f0Ref = osc->getFreq();
    orderRef = osc->getOrder();
    teethRef = osc->getTeeth();
    phaseRef = osc->getPhaseOffset();
    rRef = osc->getRadius();

    drawPoly();
    
    startTimerHz(30);
}


OscComponent::~OscComponent()
{
	delete touchHandler;
	delete osc;
}

//==============================================================================

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

void OscComponent::renderTouchPoints(Graphics& g)
{
    g.setColour(touchIndicatorCol);
    g.setOpacity(touchIndicatorAlpha);
    for(int i = 0; i < touchHandler->getNumPoints(); i++)
    {
        g.drawEllipse(touchHandler->getTouchPos(i).x-touchIndicatorSize/2, touchHandler->getTouchPos(i).y-touchIndicatorSize/2, touchIndicatorSize, touchIndicatorSize, touchIndicatorThickness);
        
        if(i == 1 || i == 2) // 2 or 3 touchpoints
        {
            g.drawDashedLine(Line<float>(touchHandler->getTouchPos(0), touchHandler->getTouchPos(i)), touchIndicatorDash, 2, touchIndicatorThickness, 0);
        }
    }
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

        //setBounds(getX()-compSize/2, getY()-compSize/2, compSize*2, compSize*2); // resize component to facilitate interactions
        //drawPoly();
    }
}

void OscComponent::mouseUp(const MouseEvent& e)
{
    touchHandler->rmTouchPoint(e);

    // update parameter reference for incremental change
    f0Ref = osc->getFreq();
    orderRef = osc->getOrder();
    teethRef = osc->getTeeth();
    phaseRef = osc->getPhaseOffset();
    rRef = osc->getRadius();



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
            osc->updateRadius(rRef + touchHandler->getRadiusDelta()); // ref + delta
            osc->updateOrder(orderRef + touchHandler->getAngleDelta()*20);

            osc->generateWavetable();
            drawPoly(); // re-draw polygon
            break;
        case 3:
            osc->updateTeeth(teethRef - touchHandler->getRadiusDelta());
            osc->updatePhaseOffset(phaseRef - touchHandler->getAngleDelta()*2);

            osc->generateWavetable();
            drawPoly();
            break;
        default:
            break;
    }
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

    polyPath.lineTo(mapToScreenCoords(Point<float>(osc->getDrawCoords(0)))); // connect last and first point to fill gap
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
