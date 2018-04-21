

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
    //int id = 0;
    //for(int i=0; i<oscillatorBank.size(); i++) // iterates through the oscillator instances
    //{
    //    oscillatorBank.at(i)->id = id; // reassigns ids to all instances
    //    id++;
    //}

    //oscInstance* o = new oscInstance(id, p, this);
    OscComponent* oscComp = new OscComponent(p);
    oscillatorBank.add(oscComp);

    addAndMakeVisible(oscComp);

    repaint();
}

void MainComponent::removeOscillator(int id)
{
    //int cId = getIndexOfChildComponent(oscillatorBank(id)->oscComp);
    //removeChildComponent(cId)
    //oscillatorBank.erase(id);
}

void MainComponent::childrenMarkedAsActive()
{
    setActiveComponent();
}

void MainComponent::setActiveComponent()
{
    //for(int i=0; i<oscillatorBank.size(); i++)
    //{
    //    if(oscillatorBank.at(i)->oscComp->activeFlag)
    //        oscillatorBank.at(i)->oscComp->setActive(true);
    //    else
    //        oscillatorBank.at(i)->oscComp->setActive(false);
    //}

}

//=============================================================================

void MainComponent::mouseDoubleClick(const MouseEvent &event)
{
    createOscillator(event.position);
}


void MainComponent::mouseDown(const MouseEvent& e)
{

}

void MainComponent::mouseDrag(const MouseEvent& e)
{

}

void MainComponent::mouseUp(const MouseEvent& e)
{

}

void MainComponent::mouseMagnify(const MouseEvent &event, float scaleFactor)
{

}

