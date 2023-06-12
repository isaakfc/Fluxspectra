/*
  ==============================================================================

    BandControls.h
    Created: 7 Jun 2023 5:25:12pm
    Author:  Isaac Faraz-Chandler

  ==============================================================================
*/

#pragma once

struct LowerBand : public juce::Component, public juce::Timer
{
    LowerBand(Assesment2AudioProcessor& audioProcessor) : processor(audioProcessor), thresholdDial("THRESHOLD"," Db"), ratioDial("RATIO",":1"), attackDial("ATTACK", " MS"),releaseDial("RELEASE", "MS"), kneeDial("KNEE", "")
    {
        addAndMakeVisible(verticalMeterL);
        addAndMakeVisible(verticalMeterR);
        fader.setSliderStyle(juce::Slider::LinearVertical);
        fader.setLookAndFeel(&myCustomLNF2);
        addAndMakeVisible(fader);
        addAndMakeVisible(thresholdDial);
        addAndMakeVisible(ratioDial);
        addAndMakeVisible(attackDial);
        addAndMakeVisible(releaseDial);
        addAndMakeVisible(kneeDial);
        startTimerHz(24);
        fader.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 30, 15);
        fader.setColour (juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
        fader.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
        inputGain = std::make_unique<Attachment>(processor.parameters,ParamIDs::gainLb,fader);
        thresholdAttachment = std::make_unique<Attachment>(processor.parameters,ParamIDs::thresholdLb,thresholdDial.getSlider());
        ratioAttachment = std::make_unique<Attachment>(processor.parameters,ParamIDs::ratioLb,ratioDial.getSlider());
        attackAttachment = std::make_unique<Attachment>(processor.parameters,ParamIDs::attackLb,attackDial.getSlider());
        releaseAttachment = std::make_unique<Attachment>(processor.parameters,ParamIDs::releaseLb,releaseDial.getSlider());
        kneeAttachment = std::make_unique<Attachment>(processor.parameters,ParamIDs::kneeLb,kneeDial.getSlider());
    }

    void resized() override
    {

        auto bounds = getLocalBounds().removeFromRight(30);
        bounds.removeFromTop(3);
        fader.setBounds(bounds);
        bounds = bounds.withTrimmedBottom(5);
        bounds.removeFromTop(20);
        verticalMeterL.setBounds(bounds.removeFromLeft(10));
        verticalMeterR.setBounds(bounds.removeFromRight(10));
        int dialSize = 90;
        bounds = getLocalBounds();
        bounds = bounds.removeFromLeft(dialSize);
        bounds.reduce(5, 3);
        thresholdDial.setBounds(bounds);
        bounds = getLocalBounds();
        bounds = bounds.removeFromLeft(dialSize*2);
        bounds = bounds.removeFromRight(dialSize);
        bounds.reduce(5, 3);
        ratioDial.setBounds(bounds);
        bounds = getLocalBounds();
        bounds = bounds.removeFromLeft(dialSize*3);
        bounds = bounds.removeFromRight(dialSize);
        bounds.reduce(5, 3);
        attackDial.setBounds(bounds);
        bounds = getLocalBounds();
        bounds = bounds.removeFromLeft(dialSize*4);
        bounds = bounds.removeFromRight(dialSize);
        bounds.reduce(5, 3);
        releaseDial.setBounds(bounds);
        kneeDial.setBounds(bounds);
        bounds = getLocalBounds();
        bounds = bounds.removeFromLeft(dialSize*5);
        bounds = bounds.removeFromRight(dialSize);
        bounds.reduce(5, 3);
        kneeDial.setBounds(bounds);
    }
    
    void paint(juce::Graphics& g) override
    {
        auto bounds = getLocalBounds();
        g.setColour(juce::Colours::white.darker(0.3));
        g.fillAll();


        bounds.reduce(3, 3);
        g.setColour(MyColours::background);
        g.fillRoundedRectangle(bounds.toFloat(), 5);
        juce::Font font3 ("Helvetica", 20.0f, juce::Font::plain);
        g.setFont(font3);
        g.setFont(9.f);
        g.setColour(juce::Colours::black);
        g.drawFittedText("Db", getWidth()-25, 6, 20, 30, juce::Justification::centred, 1);
//        g.fillAll(juce::Colours::black.brighter(0.5));

    }
    void timerCallback() override
    {
        verticalMeterL.setLevel(processor.getRMSLb(0));
        verticalMeterR.setLevel(processor.getRMSLb(1));
        verticalMeterL.repaint();
        verticalMeterR.repaint();
    }

private:
    Assesment2AudioProcessor& processor;
    Gui::VerticalMeter verticalMeterL, verticalMeterR;
    juce::CustomFader myCustomLNF2;
    juce::Slider fader;
    juce::DialBc thresholdDial, ratioDial, attackDial, releaseDial, kneeDial;

    
    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<Attachment> inputGain, thresholdAttachment, ratioAttachment, attackAttachment, releaseAttachment, kneeAttachment;
};

struct UpperBand : public juce::Component, public juce::Timer
{
public:
    UpperBand(Assesment2AudioProcessor& audioProcessor) : processor(audioProcessor), thresholdDial("THRESHOLD"," Db"), ratioDial("RATIO",":1"), attackDial("ATTACK", " MS"),releaseDial("RELEASE", "MS"), kneeDial("KNEE", "")
    {
        addAndMakeVisible(verticalMeterL);
        addAndMakeVisible(verticalMeterR);
        fader.setSliderStyle(juce::Slider::LinearVertical);
        fader.setLookAndFeel(&myCustomLNF2);
        addAndMakeVisible(fader);
        addAndMakeVisible(thresholdDial);
        addAndMakeVisible(ratioDial);
        addAndMakeVisible(attackDial);
        addAndMakeVisible(releaseDial);
        addAndMakeVisible(kneeDial);
        startTimerHz(24);
        fader.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 30, 15);
        fader.setColour (juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
        fader.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
        inputGain = std::make_unique<Attachment>(processor.parameters,ParamIDs::gainHb,fader);
        thresholdAttachment = std::make_unique<Attachment>(processor.parameters,ParamIDs::thresholdHb,thresholdDial.getSlider());
        ratioAttachment = std::make_unique<Attachment>(processor.parameters,ParamIDs::ratioHb,ratioDial.getSlider());
        attackAttachment = std::make_unique<Attachment>(processor.parameters,ParamIDs::attackHb,attackDial.getSlider());
        releaseAttachment = std::make_unique<Attachment>(processor.parameters,ParamIDs::releaseHb,releaseDial.getSlider());
        kneeAttachment = std::make_unique<Attachment>(processor.parameters,ParamIDs::kneeHb,kneeDial.getSlider());
    }

    void resized() override
    {
        auto bounds = getLocalBounds().removeFromRight(30);
        bounds.removeFromTop(3);
        fader.setBounds(bounds);
        bounds = bounds.withTrimmedBottom(5);
        bounds.removeFromTop(20);
        verticalMeterL.setBounds(bounds.removeFromLeft(10));
        verticalMeterR.setBounds(bounds.removeFromRight(10));
        int dialSize = 90;
        bounds = getLocalBounds();
        bounds = bounds.removeFromLeft(dialSize);
        bounds.reduce(5, 3);
        thresholdDial.setBounds(bounds);
        bounds = getLocalBounds();
        bounds = bounds.removeFromLeft(dialSize*2);
        bounds = bounds.removeFromRight(dialSize);
        bounds.reduce(5, 3);
        ratioDial.setBounds(bounds);
        bounds = getLocalBounds();
        bounds = bounds.removeFromLeft(dialSize*3);
        bounds = bounds.removeFromRight(dialSize);
        bounds.reduce(5, 3);
        attackDial.setBounds(bounds);
        bounds = getLocalBounds();
        bounds = bounds.removeFromLeft(dialSize*4);
        bounds = bounds.removeFromRight(dialSize);
        bounds.reduce(5, 3);
        releaseDial.setBounds(bounds);
        kneeDial.setBounds(bounds);
        bounds = getLocalBounds();
        bounds = bounds.removeFromLeft(dialSize*5);
        bounds = bounds.removeFromRight(dialSize);
        bounds.reduce(5, 3);
        kneeDial.setBounds(bounds);
    }
    void paint(juce::Graphics& g) override
    {
//        g.fillAll(juce::Colours::black.brighter(0.8));
        auto bounds = getLocalBounds();
        g.setColour(juce::Colours::white.darker(0.3));
        g.fillAll();


        bounds.reduce(3, 3);
        g.setColour(MyColours::background);
        g.fillRoundedRectangle(bounds.toFloat(), 5);
        juce::Font font3 ("Helvetica", 20.0f, juce::Font::plain);
        g.setFont(font3);
        g.setFont(9.f);
        g.setColour(juce::Colours::black);
        g.drawFittedText("Db", getWidth()-25, 6, 20, 30, juce::Justification::centred, 1);

    }
    void timerCallback() override
    {
        verticalMeterL.setLevel(processor.getRMSUb(0));
        verticalMeterR.setLevel(processor.getRMSUb(1));
        verticalMeterL.repaint();
        verticalMeterR.repaint();
    }

private:
    Assesment2AudioProcessor& processor;
    Gui::VerticalMeter verticalMeterL, verticalMeterR;
    juce::CustomFader myCustomLNF2;
    juce::Slider fader;
    juce::DialBc thresholdDial, ratioDial, attackDial, releaseDial, kneeDial;
    
    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<Attachment> inputGain, thresholdAttachment, ratioAttachment, attackAttachment, releaseAttachment, kneeAttachment;
};


struct BandControls : juce::Component
{
    BandControls(Assesment2AudioProcessor& audioProcessor) : processor(audioProcessor), tabs(juce::TabbedButtonBar::Orientation::TabsAtLeft)
    {
        tabs.addTab("LOW BAND", juce::Colours::lightgrey, new LowerBand(audioProcessor), true);
        tabs.addTab("HIGH BAND", MyColours::blue.withAlpha(0.2f), new UpperBand(audioProcessor), true);
        tabs.setLookAndFeel(&myCustomLNFTab);
        addAndMakeVisible(tabs);
    }
    
    void paint(juce::Graphics& g) override
    {


    }
    
    void resized() override
    {
        tabs.setBounds(getLocalBounds());
    }
    
private:
    Assesment2AudioProcessor& processor;
    juce::TabLookAndFeel myCustomLNFTab;
    juce::TabbedComponent tabs;
    
};
