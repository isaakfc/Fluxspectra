/*
  ==============================================================================

    GlobalControls.h
    Created: 5 Jun 2023 2:32:08pm
    Author:  Isaac Faraz-Chandler

  ==============================================================================
*/

#pragma once

enum RadioButtonIds
   {
       CompressionTypeButtons = 1001,
       DetectionTypeButtons
   };


struct GlobalControls : juce::Component
{
    GlobalControls(juce::AudioProcessorValueTreeState& apvts) : mixSlider("MIX", " %"), crossoverSlider("CROSSOVER", " HZ")
    {
        lookAhead.setSliderStyle(juce::Slider::LinearHorizontal);
        lookAhead.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 40, 50);
        lookAhead.setTextValueSuffix(" Ms");
        mixSlider.setDialSide();
        crossoverSlider.setDialSide();
        addAndMakeVisible(mixSlider);
        addAndMakeVisible(crossoverSlider);
        feedbackButton.setButtonText("FEEDBACK");
        sidechainButton.setButtonText("SIDECHAIN");
        delayButton.setButtonText("LOOK-AHEAD");
        compressionButton.setLookAndFeel(&customToggleLAF);
        limitingButton.setLookAndFeel(&customToggleLAF);
        rmsButton.setLookAndFeel(&customToggleLAF);
        peakButton.setLookAndFeel(&customToggleLAF);
        delayButton.setLookAndFeel(&customToggleLAF);
        addAndMakeVisible(delayButton);
        addAndMakeVisible(feedbackButton);
        addAndMakeVisible(sidechainButton);
        addAndMakeVisible(compressionButton);
        addAndMakeVisible(limitingButton);
        addAndMakeVisible(rmsButton);
        addAndMakeVisible(peakButton);
        addAndMakeVisible(lookAhead);
        compressionButton.onClick = [this] { updateToggleState (&compressionButton);   };
        limitingButton.onClick = [this] { updateToggleState (&limitingButton); };
        peakButton.onClick = [this] { updateToggleState (&peakButton);   };
        rmsButton.onClick = [this] { updateToggleState (&rmsButton);   };
        compressionButton.setClickingTogglesState (true);
        limitingButton.setClickingTogglesState (true);
        rmsButton.setClickingTogglesState (true);
        peakButton.setClickingTogglesState (true);
        
        
        compressionButton  .setRadioGroupId (CompressionTypeButtons);
        limitingButton.setRadioGroupId (CompressionTypeButtons);
        rmsButton.setRadioGroupId(DetectionTypeButtons);
        peakButton.setRadioGroupId(DetectionTypeButtons);

        
        mixAttachment = std::make_unique<Attachment>(apvts,ParamIDs::mix,mixSlider.getSlider());
        crossoverAttachment = std::make_unique<Attachment>(apvts,ParamIDs::crossover,crossoverSlider.getSlider());
        feedbackAttachment = std::make_unique<AttachmentButton>(apvts, ParamIDs::feedback, feedbackButton);
        sidechainAttachment = std::make_unique<AttachmentButton>(apvts, ParamIDs::source, sidechainButton);
        compressionAttachment = std::make_unique<AttachmentButton>(apvts, ParamIDs::mode, compressionButton);
        limitAttachment = std::make_unique<AttachmentButton>(apvts, ParamIDs::limitOn, limitingButton);
        peakAttachment = std::make_unique<AttachmentButton>(apvts, ParamIDs::detection, peakButton);
        RMSAttachment = std::make_unique<AttachmentButton>(apvts, ParamIDs::detectionRMS, rmsButton);
        delayAttachment = std::make_unique<Attachment>(apvts,ParamIDs::delay,lookAhead);
        delayButtonAttachment = std::make_unique<AttachmentButton>(apvts, ParamIDs::delayButton, delayButton);
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
        auto bounds = getLocalBounds();
        bounds = bounds.removeFromBottom(120);
        using namespace juce;
        auto bounds1 = bounds.removeFromBottom(bounds.getHeight() / 2);
        bounds1.reduce(3, 0);
        auto bounds2 = bounds.removeFromTop(bounds.getHeight());
        bounds2.reduce(3, 0);
        mixSlider.setBounds(bounds1);
        crossoverSlider.setBounds(bounds2);
//        crossoverLabel.setBounds(20, 10, 100, 12);
        bounds = getLocalBounds();
        bounds = bounds.removeFromTop(70);
        bounds.reduce(20, 20);
        feedbackButton.setBounds(bounds);
        bounds = getLocalBounds();
        bounds = bounds.removeFromTop(98);
        bounds = bounds.removeFromBottom(70);
        bounds.reduce(20, 20);
        sidechainButton.setBounds(bounds);
        compressionButton.setBounds (10, 91, (getWidth() - 15) / 2, 33);
        limitingButton  .setBounds (10 + ((getWidth() - 15) / 2), 91, (getWidth() - 15) / 2, 33);
        peakButton.setBounds (10, 134, (getWidth() - 15) / 2, 33);
        rmsButton  .setBounds (10 + ((getWidth() - 15) / 2), 134, (getWidth() - 15) / 2, 33);
        lookAhead.setBounds(10, 218, (getWidth() - 15), 15);
        delayButton.setBounds(15, 180, (getWidth()) -30, 26);
    }
    
    void updateToggleState (juce::Button* button)
    {

    }
    
private:
    
    juce::CustomToggle customToggleLAF;
    juce::CustomFader lookAheadLAF;
    juce::Slider lookAhead;
    juce::DialBc mixSlider, crossoverSlider;
    juce::ToggleTextButton feedbackButton, sidechainButton,delayButton;
    juce::ComboBox myComboBox;
    juce::TextButton compressionButton   { "COMPRESS" },limitingButton { "LIMIT" }, rmsButton{"RMS"}, peakButton{"PEAK"};
    

    
    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<Attachment> crossoverAttachment,
                                mixAttachment, delayAttachment;
    using AttachmentButton = juce::AudioProcessorValueTreeState::ButtonAttachment;
    std::unique_ptr<AttachmentButton> feedbackAttachment, sidechainAttachment, compressionAttachment, limitAttachment, peakAttachment, RMSAttachment, delayButtonAttachment;

};
