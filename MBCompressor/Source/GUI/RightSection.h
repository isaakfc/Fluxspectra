/*
  ==============================================================================

    RightSection.h
    Created: 4 Jun 2023 10:47:31pm
    Author:  Isaac Faraz-Chandler

  ==============================================================================
*/

#pragma once
#include "CustomUI.h"

//struct RightSection : juce::Component
//{
//    RightSection(juce::Colour colour)
//    {
//        customColour = colour;
//    }
//
//    void paint(juce::Graphics& g) override
//    {
//        g.fillAll(customColour);
//
//    }
//
//    juce::Colour customColour;
//};


struct RightSection : juce::Component, public juce::Timer
{
    RightSection(juce::Colour colour, Assesment2AudioProcessor& audioProcessor) : processor(audioProcessor)
    {

        customColour = colour;
        addAndMakeVisible(verticalMeterL);
        addAndMakeVisible(verticalMeterR);
        fader.setSliderStyle(juce::Slider::LinearVertical);
        fader.setLookAndFeel(&myCustomLNF2);
        addAndMakeVisible(fader);
        startTimerHz(24);
        inputGain = std::make_unique<Attachment>(processor.parameters,ParamIDs::makeup,fader);
        fader.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 70, 20);
        fader.setColour (juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);



    }
    void paint(juce::Graphics& g) override
    {
        g.fillAll(customColour);
        juce::Font font3 ("Helvetica", 20.0f, juce::Font::plain);
        g.setFont(font3);
        g.setFont(9.f);
        g.setColour(juce::Colours::white);
        g.drawFittedText("Db", 5, 8, 20, 30, juce::Justification::centred, 1);

    }
    void timerCallback() override
    {
        verticalMeterL.setLevel(processor.getRMSPost(0));
        verticalMeterR.setLevel(processor.getRMSPost(1));
        verticalMeterL.repaint();
        verticalMeterR.repaint();
    }
    void resized() override
    {
        auto bounds = getLocalBounds();
        fader.setBounds(bounds);
        bounds.removeFromTop(25);
        verticalMeterL.setBounds(bounds.removeFromLeft(10));
        verticalMeterR.setBounds(bounds.removeFromRight(10));
        
        
    }

private:
    juce::Colour customColour;
    juce::CustomFader myCustomLNF2;
    Gui::VerticalMeter verticalMeterL, verticalMeterR;
    Assesment2AudioProcessor& processor;
    juce::Slider fader;
    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<Attachment> inputGain;

};
