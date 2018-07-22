

#include "OscComponent.h"

OscComponent::OscComponent(const Point<float>& p, int fs)
: osc(new Oscillator(fs)), touchHandler(new TouchHandler()),
touchIndicatorSize(50), touchIndicatorThickness(1), touchIndicatorAlpha(0.2f), touchIndicatorCol(Colours::white), dashFrame(0),
compSize(300), lineThickness(5), col(Colour().fromHSV(Random().nextFloat(), 1.0f, 1.0f, 1.0f)), alphaRange(Range<float>(0.2f, 0.9f)), regressionRange(Range<float>(1.0f, 100.0f)),
refreshRate(30), idleCounter(0)
{
	setBounds(p.x-compSize/2, p.y-compSize/2, compSize, compSize);
    size = compSize-50;
    
    tweakIndicatorSize = compSize*0.85;

    f0Ref = regressionRange.getStart();
    orderRef = osc->getOrder();
    teethRef = osc->getTeeth();
    phaseRef = osc->getPhaseOffset();
    rRef = osc->getRadius();

    setSaturation(float(osc->getFreq())/osc->getFreqLimits().getEnd()); // set saturation according to f0

    drawPoly();
    
    startTimerHz(refreshRate);
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

}

void OscComponent::setInactive()
{
    active = false;
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

    for(int i = 0; i < touchHandler->getNumPoints(); i++) // touch indicator circle for all touch points
    {
        g.drawEllipse(touchHandler->getTouchPos(i).x-touchIndicatorSize/2, touchHandler->getTouchPos(i).y-touchIndicatorSize/2, touchIndicatorSize, touchIndicatorSize, touchIndicatorThickness);
        
    }

    if(touchHandler->getNumPoints() == 2) // 2 touchpoints
    {
        g.drawDashedLine(Line<float>(touchHandler->getTouchPos(0), touchHandler->getTouchPos(1)), touchIndicatorDash, 2, touchIndicatorThickness, 0);
    }
    else if(touchHandler->getNumPoints() == 3) // 3 touchpoints
    {
        // triangle outline
        g.drawDashedLine(Line<float>(touchHandler->getTouchPos(0), touchHandler->getTouchPos(1)), touchIndicatorDash, 2, touchIndicatorThickness, 0);
        g.drawDashedLine(Line<float>(touchHandler->getTouchPos(0), touchHandler->getTouchPos(2)), touchIndicatorDash, 2, touchIndicatorThickness, 0);
        g.drawDashedLine(Line<float>(touchHandler->getTouchPos(1), touchHandler->getTouchPos(2)), touchIndicatorDash, 2, touchIndicatorThickness, 0);
        // triangle fill
        g.setOpacity(touchIndicatorAlpha*0.1);
        Path tri;
        tri.addTriangle(touchHandler->getTouchPos(0), touchHandler->getTouchPos(1), touchHandler->getTouchPos(2));
        g.fillPath(tri);
    }
    
}
//==============================================================================

void OscComponent::setBrightness(const float& brightness)
{
    col = col.withBrightness(brightness);
}

void OscComponent::setSaturation(const float& saturation)
{
    col = col.withSaturation(saturation);
}

void OscComponent::setTransparency(const float& alpha)
{
    setAlpha(alphaRange.clipValue(alpha));
}

//==============================================================================

void OscComponent::timerCallback()
{   
    if(touchHandler->getNumPoints() == 1)
        idleCounter++; // increment idle time counter 

	repaint();
}

//==============================================================================

void OscComponent::paint(Graphics& g)
{
    renderSelectionIndicator(g);
    renderPoly(g);

    if(idleCounter >= refreshRate/2)
    {
        setTransparency(1.0f); // render full opaque on interaction
        renderTweakIndicator(g);
    }
    else
        setTransparency(*ramp); // change transparency according to envelope

}
void OscComponent::resized()
{

}

//==============================================================================

void OscComponent::mouseDown(const MouseEvent& e)
{
    idleCounter = 0; // reset idle time counter
    touchHandler->addTouchPoint(e);
    
    if(touchHandler->getNumPoints() == 1) // selection and dragging of oscillators
    {
        markAsActive();

        setBounds(getX()-compSize/2, getY()-compSize/2, compSize*2, compSize*2); // resize component to facilitate interactions
        drawPoly();
    }
}

void OscComponent::mouseUp(const MouseEvent& e)
{
    idleCounter = 0; // reset idle time counter
    touchHandler->rmTouchPoint(e);

    // update parameter reference for incremental change
    orderRef = osc->getOrder();
    teethRef = osc->getTeeth();
    phaseRef = osc->getPhaseOffset();
    rRef = osc->getRadius();

    if(touchHandler->getNumPoints() == 0)
    {
        setBounds(getX()+compSize/2, getY()+compSize/2, compSize, compSize); // reset size
        drawPoly();
    }
}

void OscComponent::mouseDrag(const MouseEvent& e)
{
    touchHandler->updatePoints(e);

    switch(touchHandler->getNumPoints()) // mapping based on number of touch points
    {
        case 1:
            if(idleCounter >= refreshRate/2) // exceeding 0.5 second idle time
            {
                touchHandler->sampleTouchPointCoordinate(e); // sample the coordinates of the touch point over time
                f0Ref = regressionRange.clipValue(f0Ref + touchHandler->getCircularRegression());

                osc->updateFreq(TouchHandler::linToLog(f0Ref, regressionRange, Range<float>(osc->getFreqLimits().getStart(), osc->getFreqLimits().getEnd())));
                std::cout << "lin: " << f0Ref << std::endl;
                setSaturation(float(osc->getFreq())/osc->getFreqLimits().getEnd());
            }
            else
            {
                idleCounter = 0; // reset idle time counter
                setCentrePosition(e.getScreenX(), e.getScreenY());
            }
            break;
        case 2:
            osc->updateRadius(rRef + touchHandler->getAnchorRadiusDelta()); // ref + delta
            osc->updateOrder(orderRef + touchHandler->getAnchorAngleDelta()*20);

            osc->generateWavetable();
            drawPoly(); // re-draw polygon
            break;
        case 3:
            osc->updateTeeth(teethRef - touchHandler->getTriAreaDelta());
            osc->updatePhaseOffset(phaseRef - touchHandler->getTriRotationDelta()*4);

            osc->generateWavetable();
            drawPoly();
            break;
        default:
            break;
    }
}
//==============================================================================

void OscComponent::mapRamp(float* amp)
{
    ramp = amp;
}

//==============================================================================

void OscComponent::renderSelectionIndicator(Graphics& g)
{
    if(active)
    {
        // selection indicator graphics
    }
}

void OscComponent::renderTweakIndicator(Graphics& g)
{
        Path tweakCircle;
        tweakCircle.addEllipse(compSize-tweakIndicatorSize/2, compSize-tweakIndicatorSize/2, tweakIndicatorSize, tweakIndicatorSize);
        g.setColour(touchIndicatorCol);
        g.setOpacity(touchIndicatorAlpha);
        PathStrokeType tweakStroke = PathStrokeType(touchIndicatorThickness/2);
        dashFrame++;
        tweakStroke.createDashedStroke(tweakCircle, tweakCircle, tweakIndicatorDash[int(floor(dashFrame/(refreshRate/5))) % 4], 4);
        g.strokePath(tweakCircle, tweakStroke);
}

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
