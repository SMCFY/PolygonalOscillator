

#include "Oscilloscope.h"

Oscilloscope::Oscilloscope(int samplesPerFrame, Colour col, Point<float> pos, float width)
{
	setInterceptsMouseClicks(false, false);

	setBounds(pos.x-width/2, 0, width, getParentHeight());

	this->samplesPerFrame = samplesPerFrame;
	this->col = col;
    
    setAlpha(0.1);
}

Oscilloscope::~Oscilloscope()
{

}
//==============================================================================

void Oscilloscope::fillBuffer(float* buff)
{
	waveformSegmentBuffer.clear(); // clear buffer only

	float scaleY = getParentHeight()/samplesPerFrame; // scale along the y axis
	float scaleX = getWidth()/2; //scale along x

	for(int i=0; i<samplesPerFrame-1; i++)
        waveformSegmentBuffer.push_back(Line<float>(buff[i]*scaleX+scaleX, i*scaleY, buff[i+1]*scaleX+scaleX, (i+1)*scaleY)); // add line segments according to passed buffer

    waveformSegmentRender = waveformSegmentBuffer;
}
//==============================================================================

void Oscilloscope::paint(Graphics& g)
{   
	g.setColour(col);

	for(int i=0; i<samplesPerFrame-1; i++)
        g.drawLine(waveformSegmentRender[i], 2);
}

void Oscilloscope::resized()
{
	repaint();
}
