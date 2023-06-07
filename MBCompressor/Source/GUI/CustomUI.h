/*
  ==============================================================================

    CustomUI.h
    Created: 31 May 2023 1:59:28pm
    Author:  Isaac Faraz-Chandler

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Colours.h"




namespace juce
{
class CustomDial2 : public juce::LookAndFeel_V4
{
public:
    CustomDial2();
        
    void drawRotarySlider
    (juce::Graphics &g,int x,int y,int width,int height,float sliderPos,float rotaryStartAngle,float rotaryEndAngle,juce::Slider &slider) override;
    void drawLabel (juce::Graphics& g, juce::Label& label) override;
    void drawTextEditorOutline (juce::Graphics&, int width, int height, juce::TextEditor&) override {}
private:
        float _sliderWidth;
    };


class CustomFader : public juce::LookAndFeel_V4
    {
        public:
        CustomFader();
        
        void drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height,
                                               float sliderPos,
                                               float minSliderPos,
                                               float maxSliderPos,
                                            const juce::Slider::SliderStyle style, juce::Slider& slider) override;
        void drawLabel (juce::Graphics& g, juce::Label& label) override;
        
        private:
        float _sliderWidth;
    };


class CustomTextButton : public juce::LookAndFeel_V4
    {
        public:
        CustomTextButton();
        
        void drawButtonBackground (juce::Graphics& g,
                                   juce::Button& button,
                                   const juce::Colour& backgroundColour,
                                   bool shouldDrawButtonAsHighlighted,
                                   bool shouldDrawButtonAsDown) override;
        
        juce::Font getTextButtonFont (juce::TextButton&, int buttonHeight) override;
        
    private:
    };




class CustomToggle : public juce::LookAndFeel_V4
{
public:
    void drawButtonBackground(juce::Graphics& g,
                              juce::Button& button,
                              const juce::Colour& /*backgroundColour*/,
                              bool shouldDrawButtonAsHighlighted,
                              bool shouldDrawButtonAsDown) override
    {
        // Customize the button's background
        auto buttonArea = button.getLocalBounds().toFloat();
        auto backgroundColour = button.getToggleState() ? MyColours::blue : MyColours::blackGrey;
        g.fillAll(backgroundColour);
        g.setColour(juce::Colours::black); // Set your desired border color
        g.drawRoundedRectangle(buttonArea, 6.0f, 4.0f);
    }

    void drawButtonText(juce::Graphics& g, juce::TextButton& textButton, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        // Customize the button's text
        juce::Font font3 ("Helvetica", 10.0f, juce::Font::plain);

        
        g.setFont(font3);
        g.setColour(juce::Colours::white); // Change text color here
        g.drawText(textButton.getButtonText(),
                   textButton.getLocalBounds(),
                   juce::Justification::centred,
                   true);
    }
};






class ToggleTextButton : public juce::TextButton
{
public:
    ToggleTextButton()
    {
        setClickingTogglesState(true); // Enable JUCE's built-in button state handling
        setColour(juce::TextButton::buttonColourId, MyColours::blackGrey);
        setColour(juce::TextButton::buttonOnColourId, MyColours::blue);
    }

    void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        auto bounds = getLocalBounds().toFloat().reduced(0.5f, 0.5f);
        auto cornerSize = 4.0f;

        // Fill the button with its current color
        g.setColour(findColour(getToggleState() ? juce::TextButton::buttonOnColourId : juce::TextButton::buttonColourId));
        g.fillRoundedRectangle(bounds, cornerSize);

        // Set the color of the button's outline
        g.setColour(juce::Colours::black); // Replace with your desired color

        // Draw the outline with a specified thickness
        g.drawRoundedRectangle(bounds, cornerSize, 5.0f);  // Replace 2.0f with your desired thickness

        // Set the color of the button's text
        g.setColour(juce::Colours::white); // Replace with your desired color
        

        // Set the font of the button's text
        g.setFont(juce::Font(16.0f)); // Replace 16.0f with your desired font size

        // Draw the button's text
        g.drawText(getButtonText(), bounds, juce::Justification::centred, true);
    }
};








}
