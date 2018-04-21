

#include "OscComponent.h"

OscComponent::OscComponent(const Point<float>& p)
: osc(new Oscillator()), activeFlag(false), active(false), compSize(150), lineThickness(5), col(Colour().fromRGB(Random().nextInt(255), Random().nextInt(255), Random().nextInt(255)))
{
	position = p;
	setBounds(position.x-compSize/2, position.y-compSize/2, compSize, compSize);
    size = compSize-lineThickness;

    touchHandler = new TouchHandler();

    startTimerHz(30);
}


OscComponent::~OscComponent()
{
	delete polygon;
}

//==============================================================================

void OscComponent::setPoly(const float& poly)
{

}

void OscComponent::renderPoly(Graphics& g)
{
	//if(active) // controls saturation of active/inactive instances
	//	setAlpha(1.0f);
	//else
	//	setAlpha(0.5f);


    g.setColour(col); // random RGB
    g.drawEllipse(lineThickness/2, lineThickness/2, size, size, lineThickness);
    
}

void OscComponent::setActive(const bool& a)
{
	//active = true;
	//activeFlag = false;
}

void OscComponent::markAsActive(const bool& af)
{
	//activeFlag = true;
	//toFront(true);
    //parent->childrenMarkedAsActive();
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
	markAsActive(true);
    dragger.startDraggingComponent(this, e);
	
}

void OscComponent::mouseDrag(const MouseEvent& e)
{
    dragger.dragComponent(this, e, nullptr);
}

void OscComponent::mouseUp(const MouseEvent& e)
{

}
