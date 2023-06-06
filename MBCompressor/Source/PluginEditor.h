/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "GUI/CustomUI.h"
#include "GUI/Colours.h"
#include "GUI/LogoAndTitle.h"
#include "ParamsIDs.h"
#include "GUI/VerticalMeter.h"
#include "GUI/LeftSection.h"
#include "GUI/RightSection.h"
#include "GUI/GlobalControls.h"
//==============================================================================

//const juce::Font& Assesment2AudioProcessorEditor::getWackyFont()
//{
//    static juce::Font wacky (juce::Font (juce::Typeface::createSystemTypefaceFor (BinaryData::LexendPetaRegular_ttf,
//                                                                BinaryData::LexendPetaRegular_ttfSize)));
//    return wacky;
//}



struct Placeholder : juce::Component
{
    Placeholder(juce::Colour colour)
    {
        customColour = colour;
    }
    
    void paint(juce::Graphics& g) override
    {
        g.fillAll(customColour);
    }
    
    juce::Colour customColour;
};



//struct RotarySloider : juce::Slider 
//{
//    RotarySloider():
//    juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,juce::Slider::TextEntryBoxPosition::NoTextBox)
//    {}
//    
//    
//};


juce::String getValString(juce::RangedAudioParameter& param, bool getLow, juce::String suffix);



template<
    typename Labels,
    typename ParamType,
    typename SuffixType
        >
void addLabelPairs(Labels& labels, const ParamType& param, const SuffixType& suffix)
{
    labels.clear();
    labels.add({0.f}, getValString(param, true, suffix));
    labels.add({1.f}, getValString(param, false, suffix));
}


//struct GlobalControls : juce::Component
//{
//    GlobalControls(juce::AudioProcessorValueTreeState& apvts);
//    void paint(juce::Graphics& g) override;
//    void resized() override;
//private:
//    RotarySloider mixSlider, crossoverSlider;
//
//    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
//    std::unique_ptr<Attachment> crossoverAttachment,
//                                mixAttachment;
//};








/**
*/
class Assesment2AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    Assesment2AudioProcessorEditor (Assesment2AudioProcessor&);
    ~Assesment2AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    static const juce::Font& getWackyFont();

private:
    // This reference is provided as a quick way for your editor to
//     access the processor object that created it.
    juce::CustomDial2 myCustomLNF;
    juce::CustomFader myCustomLNF2;
    Placeholder topBand, spectrogramBand, bottomBand;
    juce::Label mySliderLabel;
    Assesment2AudioProcessor& audioProcessor;
//    juce::Image logo;
    juce::Slider mySlider;
    LogoAndTextComponent logoAndText;
    leftSection leftBand;
    RightSection rightBand;
    GlobalControls globalBand { audioProcessor.parameters };
    
    
    
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Assesment2AudioProcessorEditor)
};
