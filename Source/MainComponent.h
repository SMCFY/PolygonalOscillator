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
    
    void createOscillator(const Point<float>& p); // instantiates a new oscillator - gui component pair
    void removeOscillator(const int& id);
    void setActiveComponent(const String& id); // sets the selected oscillator active, and deactivates the rest

    void mouseDoubleClick(const MouseEvent &event) override;

    void componentBroughtToFront(Component& component) override; // invoked when an oscillator instances is selected

private:

    int fs; // sampling rate

    //typedef struct oscInstance {
    //    Oscillator* osc;
    //    OscComponent* oscComp;
    //    int id;
    //    oscInstance(const int& id, const Point<float>& p, const MainComponent& mc)
    //    {
    //        osc = new Oscillator();
    //        oscComp = new OscComponent(p, mc);
    //        this->id = id;
    //    }
    //    ~oscInstance()
    //    {
    //        delete osc;
    //        delete oscComp;
    //    }
    //} oscInstance;

    OwnedArray<OscComponent> oscillatorBank;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent);
};
