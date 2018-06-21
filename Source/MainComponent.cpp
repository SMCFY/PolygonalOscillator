

#include "MainComponent.h"
bool once;
MainComponent::MainComponent()
: numberOfChannels(2), cutoff(2000), filterOrder(21)
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

    synthBuff = AudioBuffer<float>(1, samplesPerFrame); // initialize oscillator output buffer with a single channel
    
    dsp::ProcessSpec spec = {sampleRate, (uint32)samplesPerFrame, (uint32)numberOfChannels}; // filter specification
    lpf.state = dsp::FilterDesign<float>::designFIRLowpassWindowMethod (cutoff, fs, filterOrder, dsp::WindowingFunction<float>::blackman); // filter design
    lpf.prepare(spec);

}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    
    bufferToFill.clearActiveBufferRegion();
    
    for (int i = 0; i < oscillatorBank.size(); i++)
    {
        oscillatorBank[i]->synthWaveform(synthBuff.getWritePointer(0), samplesPerFrame); // pass a pointer to the output buffer

        for (int ch=0; ch < numberOfChannels; ch++)
            bufferToFill.buffer->addFrom(ch, 0, synthBuff, 0, 0, samplesPerFrame, 1.0f/float(oscillatorBank.size())); // add the oscillator outputs to the output buffer with equal weights
    }
    
    dsp::AudioBlock<float> block(*bufferToFill.buffer); // init block with output buffer
    lpf.process(dsp::ProcessContextReplacing<float>(block)); // process block

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
    
    for(int i=0; i<oscillatorBank.size(); i++)
    {
        oscillatorBank[i]->renderTouchPoints(g);
    }

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

void MainComponent::mouseDrag(const MouseEvent& e)
{
    repaint();
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
