

#include "OscComponent.h"

OscComponent::OscComponent(const Point<float>& p)
: active(true), compSize(100), col(Colour().fromRGB(Random().nextInt(255), Random().nextInt(255), Random().nextInt(255)))
{

	//setFramesPerSecond(fps);
	position = p;
	setBounds(position.x-compSize/2, position.y-compSize/2, compSize, compSize);
    setPaintingIsUnclipped(true); // enables painting outside the boundaries (this might cause optimization issues)
    std::cout << "new component created ";
}


OscComponent::~OscComponent()
{
	
}

//==============================================================================

void OscComponent::setPoly(const float& poly)
{

}

void OscComponent::renderPoly(Graphics& g)
{
	if(active) // controls saturation of active/inactive instances
		setAlpha(1.0f);
	else
		setAlpha(0.5f);


    g.setColour(col); // random RGB
    g.drawEllipse(getWidth()/2-50, getHeight()/2-50, 100, 100, 5);
}

void OscComponent::setActive(const bool& a)
{
	active = a;
}

bool OscComponent::isActive()
{
	return active;
}


//==============================================================================

void OscComponent::timerCallback()
{

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
