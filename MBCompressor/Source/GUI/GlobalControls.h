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
    GlobalControls(juce::AudioProcessorValueTreeState& apvts)
    {
        mixSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        crossoverSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        mixSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, true, 70, 15);
        crossoverSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, true, 70, 15);
        mixSlider.setTextValueSuffix("%");
        crossoverSlider.setTextValueSuffix("Hz");
        addAndMakeVisible(mixSlider);
        addAndMakeVisible(crossoverSlider);
        feedbackButton.setButtonText("FEEDBACK");
        sidechainButton.setButtonText("SIDECHAIN");
        compressionButton.setLookAndFeel(&customToggleLAF);
        limitingButton.setLookAndFeel(&customToggleLAF);
        rmsButton.setLookAndFeel(&customToggleLAF);
        peakButton.setLookAndFeel(&customToggleLAF);
        addAndMakeVisible(feedbackButton);
        addAndMakeVisible(sidechainButton);
        addAndMakeVisible(compressionButton);
        addAndMakeVisible(limitingButton);
        addAndMakeVisible(rmsButton);
        addAndMakeVisible(peakButton);
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

        
        mixAttachment = std::make_unique<Attachment>(apvts,ParamIDs::mix,mixSlider);
        crossoverAttachment = std::make_unique<Attachment>(apvts,ParamIDs::crossover,crossoverSlider);
        feedbackAttachment = std::make_unique<AttachmentButton>(apvts, ParamIDs::feedback, feedbackButton);
        sidechainAttachment = std::make_unique<AttachmentButton>(apvts, ParamIDs::source, sidechainButton);
        compressionAttachment = std::make_unique<AttachmentButton>(apvts, ParamIDs::mode, compressionButton);
        limitAttachment = std::make_unique<AttachmentButton>(apvts, ParamIDs::limitOn, limitingButton);
        peakAttachment = std::make_unique<AttachmentButton>(apvts, ParamIDs::detection, peakButton);
        RMSAttachment = std::make_unique<AttachmentButton>(apvts, ParamIDs::detectionRMS, rmsButton);
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
        juce::Font font3 ("Helvetica", 20.0f, juce::Font::plain);
        g.drawFittedText("CROSSOVER", 58, 258, 100, 10, juce::Justification::centred, 1);
        g.drawFittedText("MIX", 58, getHeight() - 15, 100, 10, juce::Justification::centred, 1);
    }
    void resized() override
    {
        auto bounds = getLocalBounds();
        bounds = bounds.removeFromBottom(180);
        using namespace juce;
        auto bounds1 = bounds.removeFromBottom(bounds.getHeight() / 2);
        bounds1.reduce(3, 0);
        auto bounds2 = bounds.removeFromTop(bounds.getHeight());
        bounds2.reduce(3, 0);
        mixSlider.setBounds(bounds1);
        crossoverSlider.setBounds(bounds2);
        crossoverLabel.setBounds(20, 10, 100, 12);
        bounds = getLocalBounds();
        bounds = bounds.removeFromTop(70);
        bounds.reduce(20, 20);
        feedbackButton.setBounds(bounds);
        bounds = getLocalBounds();
        bounds = bounds.removeFromTop(98);
        bounds = bounds.removeFromBottom(70);
        bounds.reduce(20, 20);
        sidechainButton.setBounds(bounds);
        compressionButton.setBounds (10, 95, (getWidth() - 15) / 2, 33);
        limitingButton  .setBounds (10 + ((getWidth() - 15) / 2), 95, (getWidth() - 15) / 2, 33);
        peakButton.setBounds (10, 138, (getWidth() - 15) / 2, 33);
        rmsButton  .setBounds (10 + ((getWidth() - 15) / 2), 138, (getWidth() - 15) / 2, 33);
    }
    
    void updateToggleState (juce::Button* button)
    {

    }
    
private:
    
    juce::CustomToggle customToggleLAF;
    juce::Slider mixSlider, crossoverSlider;
    juce::Label MixLabel, crossoverLabel;
    juce::ToggleTextButton feedbackButton, sidechainButton;
    juce::ComboBox myComboBox;
    juce::TextButton compressionButton   { "COMPRESS" },limitingButton { "LIMIT" }, rmsButton{"RMS"}, peakButton{"PEAK"};
    

    
    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<Attachment> crossoverAttachment,
                                mixAttachment;
    using AttachmentButton = juce::AudioProcessorValueTreeState::ButtonAttachment;
    std::unique_ptr<AttachmentButton> feedbackAttachment, sidechainAttachment, compressionAttachment, limitAttachment, peakAttachment, RMSAttachment;
    using AttachmentCombo = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    std::unique_ptr<AttachmentCombo> typeAttachment;
};
