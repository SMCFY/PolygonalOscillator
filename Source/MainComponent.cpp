

#include "MainComponent.h"

MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize(800, 600);
    setAudioChannels(2, 2);
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
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();
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

    OscComponent* oscComp = new OscComponent(p);
    oscComp->setComponentID(String(id)); // highest id goes to the most recent instance
    oscComp->addComponentListener(this);
    oscComp->setActive(); // set the newest instance as active
    addAndMakeVisible(oscComp);
    oscillatorBank.add(oscComp);

    repaint();
}

void MainComponent::removeOscillator(const int& id)
{
    //int cId = getIndexOfChildComponent(oscillatorBank(id)->oscComp);
    //removeChildComponent(cId)
    //oscillatorBank.erase(id);
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
    std::cout << "ACTIVATED";
}
