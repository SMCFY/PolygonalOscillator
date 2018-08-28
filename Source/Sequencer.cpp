

#include "Sequencer.h"

Sequencer::Sequencer()
: tempoRange(Range<int>(10, 60)), tempo(0), loopLength(2000), loopPos(0), wait(false)
{
	eventLoc = loopLength;
}

Sequencer::~Sequencer()
{

}

//==============================================================================

bool Sequencer::tick()
{
	updateCounter();

    bool trig = false;
	int interval = 60000 / tempo; // BPM to IOI

	if(fmod(msCounter, interval) < mod) // if the remainder is smaller then it was
        trig = true;

    mod = fmod(msCounter, interval); // update stored remainder
    
    return trig;
}

bool Sequencer::nudge()
{
	updateCounter();

	bool trig = false;

	if(fmod(msCounter, loopLength) < loopPos) // loop completed
		wait = false;

	loopPos = fmod(msCounter, loopLength); // update timeline position 
	if(loopPos >= eventLoc && !wait)
	{
		wait = true;
	 	trig = true;
	} 

	return trig;
}

//==============================================================================

void Sequencer::calculateTempo(const float& normVal)
{
    mod = 0;
	tempo = normVal * (tempoRange.getEnd()-tempoRange.getStart()) + tempoRange.getStart();
}

void Sequencer::calculateEventLoc(const float& normVal)
{
	eventLoc = normVal*loopLength;
}


//==============================================================================

void Sequencer::updateCounter()
{
	msCounter = Time::getMillisecondCounter();
}
