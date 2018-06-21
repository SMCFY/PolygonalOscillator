

#include "Sequencer.h"

Sequencer::Sequencer()
{

}

Sequencer::~Sequencer()
{

}

//==============================================================================

void Sequencer::updateCounter()
{
	msCounter = Time::getMillisecondCounter();
}

bool Sequencer::modCounter(const int& tempo)
{
    bool trig = false;
	int interval = 60000 / tempo; // BPM to IOI

	if(fmod(msCounter, interval) < mod) // if the remainder is smaller then it was
        trig = true;
    mod = fmod(msCounter, interval); // update stored remainder

    return trig;
}
