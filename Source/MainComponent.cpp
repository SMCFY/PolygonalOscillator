

#include "MainComponent.h"

MainComponent::MainComponent()
: numberOfChannels(2), cutoff(4000), filterOrder(21), filterWindow(dsp::WindowingFunction<float>::blackman)
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
    lpf.state = dsp::FilterDesign<float>::designFIRLowpassWindowMethod (cutoff, fs, filterOrder, filterWindow); // filter design
    lpf.prepare(spec);

}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    
    bufferToFill.clearActiveBufferRegion();
    
    for (int i = 0; i < oscillatorBank.size(); i++)
    {
        oscillatorBank[i]->oscComp->synthWaveform(synthBuff.getWritePointer(0), samplesPerFrame); // pass a pointer to the output buffer

        for (int ch=0; ch < numberOfChannels; ch++)
            bufferToFill.buffer->addFrom(ch, 0, synthBuff, 0, 0, samplesPerFrame, 1.0f/float(oscillatorBank.size())); // add the oscillator outputs to the output buffer with equal weights
    }
    
    dsp::AudioBlock<float> block(*bufferToFill.buffer); // init block with output buffer
    lpf.process(dsp::ProcessContextReplacing<float>(block)); // process block

    seq.updateCounter();
    if(seq.modCounter(60))
    {
        std::cout << "TRIGGER" << std::endl;
    }
    
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
        oscillatorBank[i]->oscComp->renderTouchPoints(g);
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
    OscInstance* o = new OscInstance(p, fs);
    
    o->oscComp->setComponentID(String(oscillatorBank.size())); // new instance gets id mathcing its future index in the array
    o->oscComp->addComponentListener(this);
    addAndMakeVisible(o->oscComp);
    oscillatorBank.add(o);

    setActiveComponent(String(oscillatorBank.size()-1));

    repaint();
}

void MainComponent::removeOscillator(const String& id)
{
    oscillatorBank.removeObject(oscillatorBank[id.getIntValue()]);
    
    int newId = 0;
    for(int i=0; i<oscillatorBank.size(); i++) // iterates through the oscillator instances
    {
        oscillatorBank[i]->oscComp->setComponentID(String(newId)); // assigns new id to all instances
        newId++;
    }

}

void MainComponent::setActiveComponent(const String& id)
{

    for(int i=0; i<oscillatorBank.size(); i++)
        oscillatorBank[i]->oscComp->setInactive(); // sets all components inactive

    oscillatorBank[id.getIntValue()]->oscComp->setActive();
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
    //oscillatorBank[component.getComponentID().getIntValue()]->tempo = component.getScreenX() / getWidth(); // change tempo according to component's screen coordinates
    
    if(component.getBottom() > getHeight()+component.getHeight()*0.4) // delete oscillator if its dragged to the bottom of the screen
    {
        String targetId = component.getComponentID();
        removeOscillator(targetId);
    }

    
}
