/*
  ==============================================================================

    Sequencer class

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Sequencer
{
public:

	Sequencer();
	~Sequencer();

	void updateCounter(); // updates the internal counter
	bool modCounter(const int& tempo); // returns a counter within a specified interval


private:

	uint32 msCounter; // millisecond counter
	int mod; // remainder from modulo

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Sequencer);
};
