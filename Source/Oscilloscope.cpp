

#include "Oscilloscope.h"

Oscilloscope::Oscilloscope(int samplesPerFrame, Colour col, Point<float> pos, float width)
{
	setInterceptsMouseClicks(false, false);

	setBounds(pos.x-width/2, 0, width, getParentHeight());

	this->samplesPerFrame = samplesPerFrame;
	this->col = col;
    
    setAlpha(0.25);
}

Oscilloscope::~Oscilloscope()
{

}
//==============================================================================

void Oscilloscope::fillBuffer()
{

}
//==============================================================================

void Oscilloscope::paint(Graphics& g)
{
    g.fillAll(col);
	//for(int i=0; i<samplesPerFrame; i++)
	//	g.drawLine(waveSegment[i])
}

void Oscilloscope::resized()
{
	repaint();
}
