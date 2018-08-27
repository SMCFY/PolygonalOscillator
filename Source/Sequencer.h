/*
  ==============================================================================

    Triggering envelopes according to given intervals or sequence.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Sequencer
{
public:

	Sequencer();
	~Sequencer();

	bool tick(); // returns ticks according to set tempo
	bool nudge(); // returns ticks according to an event specified on a timeline

	void calculateTempo(const float& normVal);
	void calculateEventLoc(const float& normVal);

private:

	void updateCounter(); // updates the internal counter

	uint32 msCounter; // millisecond counter
	int mod; // remainder from modulo
	int tempo;
	Range<int> tempoRange;

	int loopLength;
	int loopPos;
	int eventLoc;

	bool wait;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Sequencer);
};
