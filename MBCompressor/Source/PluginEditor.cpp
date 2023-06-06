/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"



template<typename T>
bool truncateKiloValue(T& value)
{
    if(value > static_cast<T>(999))
    {
        value /= static_cast<T>(1000);
        return true;
    }
    return false;
}


juce::String getValString(juce::RangedAudioParameter& param, bool getLow, juce::String suffix)
{
    juce::String str;
    
    
    auto val = getLow ? param.getNormalisableRange().start : param.getNormalisableRange().end;
    
    bool useK = truncateKiloValue(val);
    
    str << val;
    
    if (useK)
    {
        str << "k";
    }
    
    str << suffix;
    
    return str;
}




//GlobalControls::GlobalControls(juce::AudioProcessorValueTreeState& apvts)
//{
//    
//    addAndMakeVisible(mixSlider);
//    addAndMakeVisible(crossoverSlider);
//    
//    mixAttachment = std::make_unique<Attachment>(apvts,ParamIDs::mix,mixSlider);
//    crossoverAttachment = std::make_unique<Attachment>(apvts,ParamIDs::crossover,crossoverSlider);
//}


//void GlobalControls::paint(juce::Graphics &g)
//{
//    using namespace juce;
//    auto bounds = getLocalBounds();
//    g.setColour(Colours::white.darker(0.3));
//    g.fillAll();
//    
//    
//    bounds.reduce(3, 3);
//    g.setColour(MyColours::background);
//    g.fillRoundedRectangle(bounds.toFloat(), 5);
//    
//    
//    
//
//}

//void GlobalControls::resized()
//{
//    auto bounds = getLocalBounds();
//    bounds = bounds.removeFromBottom(180);
//    using namespace juce;
//
//    FlexBox flexBox;
//    flexBox.flexDirection = FlexBox::Direction::column;
//    flexBox.flexWrap = FlexBox::Wrap::noWrap;
//
//
//
//    flexBox.items.add(FlexItem(mixSlider).withFlex(1.f));
//    flexBox.items.add(FlexItem(crossoverSlider).withFlex(1.f));
//
//    flexBox.performLayout(bounds);
//
//}



//==============================================================================
Assesment2AudioProcessorEditor::Assesment2AudioProcessorEditor (Assesment2AudioProcessor& p)
: AudioProcessorEditor (&p), topBand(MyColours::black), spectrogramBand(MyColours::background),bottomBand(juce::Colours::white), audioProcessor (p), leftBand(MyColours::black, p),rightBand(MyColours::black, p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    juce::LookAndFeel::setDefaultLookAndFeel(&myCustomLNF);
    
    addAndMakeVisible(topBand);
    addAndMakeVisible(globalBand);
    addAndMakeVisible(rightBand);
    addAndMakeVisible(leftBand);
    addAndMakeVisible(bottomBand);
    addAndMakeVisible(spectrogramBand);
    
    // Custom Slider Code
//    mySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
//    mySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 15);
//    mySlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentWhite);
//    mySlider.setColour(juce::Slider::textBoxTextColourId,  MyColours::logotext);
//    juce::Font fontbold (juce::Font (juce::Typeface::createSystemTypefaceFor (BinaryData::LexendPetaBold_ttf,
//                                                                            BinaryData::LexendPetaBold_ttfSize)));
//    fontbold.setHeight(20);
//    mySliderLabel.setFont(fontbold);
//    mySliderLabel.setColour(juce::Label::textColourId, MyColours::logotext);
//    mySliderLabel.setText("CROSSOVER", juce::dontSendNotification);
//
    
    
//    addAndMakeVisible(mySliderLabel);
//    addAndMakeVisible(mySlider);
    addAndMakeVisible(logoAndText);
    
    setSize (720, 450);
}

Assesment2AudioProcessorEditor::~Assesment2AudioProcessorEditor()
{
}

//==============================================================================
void Assesment2AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    

    

}

void Assesment2AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto bounds = getLocalBounds();
    
    topBand.setBounds( bounds.removeFromTop(90) );
    leftBand.setBounds(bounds.removeFromLeft(30));
    rightBand.setBounds(bounds.removeFromRight(30));
    globalBand.setBounds(bounds.removeFromRight(150));
    bottomBand.setBounds(bounds.removeFromBottom(130));
    spectrogramBand.setBounds(bounds);
    
    
    
    bounds = getLocalBounds();
    mySlider.setBounds(bounds.getWidth() / 1.375 , bounds.getHeight() / 2 + 25, 200, 180);
    mySliderLabel.setBounds(mySlider.getX() + mySlider.getWidth() / 4.8, mySlider.getY() + mySlider.getHeight() / 1.7 , 120, 100);
    logoAndText.setBounds(topBand.getBounds());
}



