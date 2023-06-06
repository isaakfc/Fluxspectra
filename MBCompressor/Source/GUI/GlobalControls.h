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
        addAndMakeVisible(feedbackButton);
        addAndMakeVisible(sidechainButton);
        addAndMakeVisible(compressionButton);
        addAndMakeVisible(limitingButton);
        compressionButton  .onClick = [this] { updateToggleState (&compressionButton,   "Male");   };
        limitingButton.onClick = [this] { updateToggleState (&limitingButton, "Female"); };
        
        compressionButton.setClickingTogglesState (true);
        limitingButton.setClickingTogglesState (true);
        
        compressionButton  .setRadioGroupId (CompressionTypeButtons);
        limitingButton.setRadioGroupId (CompressionTypeButtons);

        
        mixAttachment = std::make_unique<Attachment>(apvts,ParamIDs::mix,mixSlider);
        crossoverAttachment = std::make_unique<Attachment>(apvts,ParamIDs::crossover,crossoverSlider);
        feedbackAttachment = std::make_unique<AttachmentButton>(apvts, ParamIDs::feedback, feedbackButton);
        sidechainAttachment = std::make_unique<AttachmentButton>(apvts, ParamIDs::source, sidechainButton);
//        limitingAttachment = std::make_unique<AttachmentButton>(apvts, ParamIDs::mode, limitingButton);
//        compressionAttachment = std::make_unique<AttachmentButton>(apvts, ParamIDs::mode, compressionButton);
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
        bounds = getLocalBounds();
        bounds = bounds.removeFromTop(bounds.getHeight() - 180);
        bounds = bounds.removeFromBottom(bounds.getHeight() - 70);
        bounds = bounds.removeFromTop(bounds.getHeight()/2);
        bounds.reduce(10, 10);
        myComboBox.setBounds(bounds);
        
        
        compressionButton  .setBounds (20, 90, getWidth() - 30, 20);
        limitingButton.setBounds (20, 110, getWidth() - 30, 20);
    }
    
    void updateToggleState (juce::Button* button, juce::String name)
    {
        auto state = button->getToggleState();
        juce::String stateString = state ? "ON" : "OFF";

        juce::Logger::outputDebugString (name + " Button changed to " + stateString);
    }
    
private:
    juce::Slider mixSlider, crossoverSlider;
    juce::Label MixLabel, crossoverLabel;
    juce::ToggleTextButton feedbackButton, sidechainButton;
    juce::ComboBox myComboBox;
    juce::TextButton compressionButton   { "Compression" },limitingButton { "Limiting" };

    
    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<Attachment> crossoverAttachment,
                                mixAttachment;
    using AttachmentButton = juce::AudioProcessorValueTreeState::ButtonAttachment;
    std::unique_ptr<AttachmentButton> feedbackAttachment, sidechainAttachment, compressionAttachment, limitingAttachment;
    using AttachmentCombo = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    std::unique_ptr<AttachmentCombo> typeAttachment;
};
