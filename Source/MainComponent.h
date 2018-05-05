/*
  ==============================================================================

    Main class for handling audio.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "OscComponent.h"

class MainComponent : public AudioAppComponent, public ComponentListener
{
public:

    MainComponent();
    ~MainComponent();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void paint(Graphics& g) override;
    void resized() override;

    void mouseDoubleClick(const MouseEvent &event) override;

    void componentBroughtToFront(Component& component) override; // called when an oscillator instances is selected
    void componentMovedOrResized (Component &component, bool wasMoved, bool wasResized) override; // called when an oscillator is dragged

private:

    int fs; // sampling rate
    int samplesPerFrame; // number of samples per frame
    float** outputFrame; // array to hold synthesized waveforms

    void createOscillator(const Point<float>& p); // instantiates a new oscillator - gui component pair
    void removeOscillator(const String& id);
    void setActiveComponent(const String& id); // sets the selected oscillator active, and deactivates the rest
    
    OwnedArray<OscComponent> oscillatorBank;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent);
};
