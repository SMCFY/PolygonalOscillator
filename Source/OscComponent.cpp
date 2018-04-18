

#include "OscComponent.h"

OscComponent::OscComponent(const Point<float>& p)
: fps(30), compSize(200)
{

	setFramesPerSecond(fps);
	position = p;
	setBounds(position.x, position.y, compSize, compSize);


    std::cout << "new component created ";
}


OscComponent::~OscComponent()
{
	
}

//==============================================================================

void OscComponent::setPoly(const float& poly)
{

}


//==============================================================================

void OscComponent::update()
{

}

//==============================================================================

void OscComponent::timerCallback()
{

}

//==============================================================================

void OscComponent::paint(Graphics& g)
{
	std::cout << "drawyacunt ";
	g.setColour(Colour().fromRGB(Random().nextInt(255), Random().nextInt(255), Random().nextInt(255))); // random RGB
	g.drawEllipse(position.x-25, position.y-25, 50, 50, 5);
}

void OscComponent::resized()
{
    repaint();
}
