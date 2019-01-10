

#include "MainComponent.h"

MainComponent::MainComponent()
: numberOfChannels(2), childSize(300),
bg(ImageFileFormat::loadFrom(BinaryData::concrete_bg_png, (size_t) BinaryData::concrete_bg_pngSize))
{
    setSize(800, 600);
    setAudioChannels(0, numberOfChannels);

    startTimerHz(30);
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

}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{  
    bufferToFill.clearActiveBufferRegion();
    
    
    for (int i = 0; i < oscillatorBank.size(); i++)
    {   
        oscillatorBank[i]->oscComp->synthWaveform(synthBuff.getWritePointer(0)); // synthesize waveform to oscillator output buffer

        if(oscillatorBank[i]->oscComp->isMouseButtonDown())
            oscillatorBank[i]->env->noteOn(); // sustain envelope in interactions
        else
            oscillatorBank[i]->env->noteOff();

        if(oscillatorBank[i]->seq->nudge()) // nudge sequencer
        {
            oscillatorBank[i]->env->trigger(); // trigger envelope
        }
        oscillatorBank[i]->env->process(synthBuff); // process oscillator output buffer with respective envelope
        // oscillatorBank[i]->lpf->process(synthBuff); // filtering

        for (int ch=0; ch < numberOfChannels; ch++)
            bufferToFill.buffer->addFrom(ch, 0, synthBuff, 0, 0, samplesPerFrame, 1.0f/float(oscillatorBank.size())); // add the oscillator outputs to the output buffer with equal weights
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
    //g.fillAll(Colour().fromRGB(30, 30, 30)); // background
    g.drawImageWithin(bg, 0, 0, getWidth(), getHeight(), RectanglePlacement::centred);
    
    for(int i=0; i<oscillatorBank.size(); i++)
    {
        oscillatorBank[i]->oscComp->renderTouchPoints(g);
        oscillatorBank[i]->oscComp->repaint();
        oscillatorBank[i]->scope->fillBuffer(synthBuff.getWritePointer(0));
        oscillatorBank[i]->scope->repaint();
    }

}

void MainComponent::resized()
{
    repaint();
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}

//=============================================================================

void MainComponent::createOscillator(const Point<float>& p)
{
    OscInstance* o = new OscInstance(p, fs, samplesPerFrame, numberOfChannels, Colour().fromHSV(Random().nextFloat(), 1.0f, 1.0f, 1.0f), childSize);
    
    o->oscComp->setComponentID(String(oscillatorBank.size())); // new instance gets id mathcing its future index in the array
    o->oscComp->addComponentListener(this);
    addAndMakeVisible(o->scope);
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
    oscillatorBank[component.getComponentID().getIntValue()]->scope->setTopLeftPosition(
        oscillatorBank[component.getComponentID().getIntValue()]->oscComp->getX() +
        oscillatorBank[component.getComponentID().getIntValue()]->oscComp->getWidth()/2 - childSize/2, 0);

    // timing
    float normCoordX = float(component.getPosition().getX()+component.getWidth()/2) / float(getWidth()); // normalised position of the components center on the x axis
    oscillatorBank[component.getComponentID().getIntValue()]->seq->calculateEventLoc(normCoordX); // change sequencing according to component's center position

    // attack, release
    float normCoordY = float(component.getPosition().getY()+component.getHeight()/2) / float(getHeight()); // normalised position of the components center on the y axis
    oscillatorBank[component.getComponentID().getIntValue()]->env->setAttackTime(normCoordY*500);
    oscillatorBank[component.getComponentID().getIntValue()]->env->setReleaseTime(normCoordY*1000);

    if(component.getBottom() > getHeight()+component.getHeight()*0.4) // delete oscillator if its dragged to the bottom of the screen
    {
        String targetId = component.getComponentID();
        removeOscillator(targetId);
    }

}

//=============================================================================

void MainComponent::timerCallback()
{
    repaint();
}
