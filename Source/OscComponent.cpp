

#include "OscComponent.h"

OscComponent::OscComponent(const Point<float>& p)
: osc(new Oscillator()), touchHandler(new TouchHandler()),
compSize(150), lineThickness(5), col(Colour().fromRGB(Random().nextInt(155)+100, Random().nextInt(155)+100, Random().nextInt(155)+100))
{
	position = p;
	setBounds(position.x-compSize/2, position.y-compSize/2, compSize, compSize);
    size = compSize-lineThickness;
    
    startTimerHz(30);
}


OscComponent::~OscComponent()
{
	delete polygon;
	delete touchHandler;
	delete osc;
}

//==============================================================================

void OscComponent::setPoly(const float& poly)
{

}

void OscComponent::renderPoly(Graphics& g)
{
    g.setColour(col); // random RGB
    g.drawEllipse(lineThickness/2, lineThickness/2, size, size, lineThickness);
    
}

void OscComponent::setActive()
{
	active = true;
    setAlpha(1.0f);
}

void OscComponent::setInactive()
{
    active = false;
    setAlpha(0.6f);
}

void OscComponent::markAsActive()
{
	toFront(true);
    std::cout << "marked as active";
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
    repaint();
}

//==============================================================================

void OscComponent::mouseDown(const MouseEvent& e)
{
    // if(onefinger only)
    //{
	markAsActive();
    dragger.startDraggingComponent(this, e);
    //}
	
}

void OscComponent::mouseDrag(const MouseEvent& e)
{
    dragger.dragComponent(this, e, nullptr);
}

void OscComponent::mouseUp(const MouseEvent& e)
{

}
