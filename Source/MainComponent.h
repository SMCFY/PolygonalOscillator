/*
  ==============================================================================

    Main class for handling audio and interactions.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>
#include "Oscillator.h"
#include "OscComponent.h"
#include "TouchHandler.h"

class MainComponent : public AudioAppComponent
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
    void removeOscillator(int id);

    void mouseDoubleClick(const MouseEvent &event) override;
    void mouseDown(const MouseEvent& e) override;
    void mouseDrag(const MouseEvent& e) override;
    void mouseUp(const MouseEvent& e) override;
    void mouseMagnify(const MouseEvent &event, float scaleFactor) override;



private:
    
    int activeId; // id to match selected oscillator
    int fs; // sampling rate
    typedef struct oscInstance {
        Oscillator* osc;
        OscComponent* oscComp;
        int id;
        oscInstance(const int& id, const Point<float>& p)
        {
            this->id = id;
            osc = new Oscillator();
            oscComp = new OscComponent(p);
        }
        ~oscInstance()
        {
            delete osc;
            delete oscComp;
        }
    } oscInstance;

    std::vector<oscInstance*> oscillatorBank; // stores the oscillator - gui component pairs


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent);
};
