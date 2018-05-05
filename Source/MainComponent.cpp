

#include "MainComponent.h"
bool once;
MainComponent::MainComponent()
: numberOfChannels(2)
{
    setSize(800, 600);
    setAudioChannels(0, numberOfChannels);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
    oscillatorBank.clear(true);
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    fs = sampleRate;
    samplesPerFrame = samplesPerBlockExpected;
    
    outputFrame = new float*[numberOfChannels];
    for (int ch = 0; ch < numberOfChannels; ch++)
        outputFrame[ch] = new float(samplesPerFrame);

    outputBuffer = AudioBuffer<float>(numberOfChannels, samplesPerFrame);
    
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    
    bufferToFill.clearActiveBufferRegion();
    
    for (int i = 0; i < oscillatorBank.size(); i++)
    {
        oscillatorBank[i]->synthWaveform(outputBuffer);

    }
    
    if(oscillatorBank.size() != 0)
        bufferToFill.buffer->copyFrom(0, 0, outputBuffer, 0, 0, 256);

}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    g.fillAll(Colour().fromRGB(30, 30, 30));

}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}

//=============================================================================

void MainComponent::createOscillator(const Point<float>& p)
{
    int id = 0;
    for(int i=0; i<oscillatorBank.size(); i++) // iterates through the oscillator instances
    {
        oscillatorBank[i]->setComponentID(String(id)); // reassigns ids to all instances
        oscillatorBank[i]->setInactive(); // deactivates existing oscillators
        id++;
    }

    OscComponent* oscComp = new OscComponent(p, fs);
    oscComp->setComponentID(String(id)); // highest id goes to the most recent instance
    oscComp->addComponentListener(this);
    oscComp->setActive(); // set the newest instance as active
    addAndMakeVisible(oscComp);
    oscillatorBank.add(oscComp);

    repaint();
}

void MainComponent::removeOscillator(const String& id)
{
    for(int i=0; i<oscillatorBank.size(); i++)
    {
        if(oscillatorBank[i]->getComponentID() == id)
            oscillatorBank.removeObject(oscillatorBank[i]);
    }
    
}

void MainComponent::setActiveComponent(const String& id)
{
    for(int i=0; i<oscillatorBank.size(); i++)
    {
        if(oscillatorBank[i]->getComponentID() == id)
            oscillatorBank[i]->setActive();
        else
            oscillatorBank[i]->setInactive();
    }

}

//=============================================================================

void MainComponent::mouseDoubleClick(const MouseEvent &event)
{
    createOscillator(event.position);
}

//=============================================================================

void MainComponent::componentBroughtToFront(Component& component)
{
    String activeId = component.getComponentID();
    setActiveComponent(activeId);
}

void MainComponent::componentMovedOrResized (Component &component, bool wasMoved, bool wasResized)
{
    if(component.getBottom() > getHeight()+component.getHeight()/4) // delete oscillator if its dragged to the bottom of the screen
    {
        String targetId = component.getComponentID();
        removeOscillator(targetId);
    }

}
