/*
  ==============================================================================

    SpectrogramBand.h
    Created: 9 Jun 2023 1:40:12pm
    Author:  Isaac Faraz-Chandler

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Colours.h"
#include "../PluginProcessor.h"

struct spectrogramBand : juce::Component
{
    spectrogramBand(Assesment2AudioProcessor& audioProcessor) : processor(audioProcessor)
    {

    }
    void paint(juce::Graphics& g) override
    {

        using namespace juce;
        auto bounds = getLocalBounds();
        g.setColour(Colours::white.darker(0.3));
        g.fillAll();
        
        
        bounds.reduce(3, 3);
        g.setColour(MyColours::background);
        g.fillRoundedRectangle(bounds.toFloat(), 5);
        g.setColour(MyColours::black);

    }

    void resized() override
    {
        
    }

private:

    Assesment2AudioProcessor& processor;
    SingleChannelSampleFifo<Assesment2AudioProcessor::BlockType>* leftChannelFifo;
    
};
