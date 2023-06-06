/*
  ==============================================================================

    LeftSection.h
    Created: 4 Jun 2023 8:40:36pm
    Author:  Isaac Faraz-Chandler

  ==============================================================================
*/

#pragma once
#include "CustomUI.h"


struct leftSection : juce::Component, public juce::Timer
{
    leftSection(juce::Colour colour, Assesment2AudioProcessor& audioProcessor) : processor(audioProcessor)
    {

        customColour = colour;
        addAndMakeVisible(horizontalMeterL);
        addAndMakeVisible(horizontalMeterR);
        fader.setSliderStyle(juce::Slider::LinearVertical);
        fader.setLookAndFeel(&myCustomLNF2);
        addAndMakeVisible(fader);
        startTimerHz(24);
        inputGain = std::make_unique<Attachment>(processor.parameters,ParamIDs::gain,fader);



    }
    void paint(juce::Graphics& g) override
    {
        g.fillAll(customColour);

    }
    void timerCallback() override
    {
        horizontalMeterL.setLevel(processor.getRMSValue(0));
        horizontalMeterR.setLevel(processor.getRMSValue(1));
        horizontalMeterL.repaint();
        horizontalMeterR.repaint();
    }
    void resized() override
    {
        auto bounds = getLocalBounds();
        fader.setBounds(bounds);
        horizontalMeterL.setBounds(bounds.removeFromLeft(10));
        horizontalMeterR.setBounds(bounds.removeFromRight(10));
        
        
    }

private:
    juce::Colour customColour;
    juce::CustomFader myCustomLNF2;
    Gui::HorizontalMeter3 horizontalMeterL, horizontalMeterR;
    Assesment2AudioProcessor& processor;
    juce::Slider fader;
    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<Attachment> inputGain;

};


