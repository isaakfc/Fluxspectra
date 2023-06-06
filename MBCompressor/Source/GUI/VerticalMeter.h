/*
  ==============================================================================

    VerticalMeter.h
    Created: 3 Jun 2023 10:51:35pm
    Author:  Isaac Faraz-Chandler

  ==============================================================================
*/

#pragma once


#include <JuceHeader.h>

namespace Gui
{
    class HorizontalMeter : public juce::Component
    {
    public:
        void paint(juce::Graphics& g) override
        {
            auto bounds = getLocalBounds().toFloat();
            g.setColour(juce::Colours::white.withBrightness(0.4f));
            g.fillRoundedRectangle(bounds, 5.f);
            g.setColour(juce::Colours::white);
            const auto scaledY = juce::jmap(level, -60.f,6.f,0.f,static_cast<float>(getHeight()));
//            g.fillRoundedRectangle(5.f, bounds.removeFromBottom(scaledY));
            g.fillRoundedRectangle(bounds.removeFromBottom(scaledY), 5.f);
            
        }
        void setLevel(const float value)
        {
            level = value;
        }
        
    private:
        float level = -60.f;
        
        
        
    };





    class HorizontalMeter2 : public juce::Component
    {
    public:
        void paint(juce::Graphics& g) override
        {
            auto bounds = getLocalBounds().toFloat().reduced(3.f);

            g.setColour(MyColours::black);
            g.fillRect(bounds);

            g.setGradientFill(gradient);
            const auto scaledY = juce::jmap(level, -60.f, 6.f, 0.f, static_cast<float>(getHeight()));
            g.fillRect(bounds.removeFromBottom(scaledY));
            
        }
        void setLevel(const float value)
        {
            level = value;
        }
        void resized() override
        {
            const auto bounds = getLocalBounds().toFloat();
            gradient = juce::ColourGradient{ juce::Colours::white, bounds.getBottomLeft(), MyColours::blue, bounds.getTopLeft(), false };
            gradient.addColour(0.5, MyColours::blue);
        }
        
        
    private:
        float level = -60.f;
        juce::ColourGradient gradient{};
    
    
};

class HorizontalMeter3 : public juce::Component
{
public:
    void paint(juce::Graphics& g) override
    {
        auto bounds = getLocalBounds().toFloat().reduced(3.f);

        g.setColour(juce::Colours::white.withBrightness(0.4f));
        g.fillRoundedRectangle(bounds, 0.f);
        g.setColour(juce::Colours::white);
        const auto scaledY = juce::jmap(level, -60.f,6.f,0.f,static_cast<float>(getHeight()));
//            g.fillRoundedRectangle(5.f, bounds.removeFromBottom(scaledY));
        bounds = bounds.removeFromBottom(scaledY);
        g.fillRoundedRectangle(bounds, 0.f);
        
        if (level > 0)
        {
            float temp = level - 60.f;
            const auto scaledYTop = juce::jmap(temp, -60.f,6.f,0.f,static_cast<float>(getHeight()));
            bounds = bounds.removeFromTop(scaledYTop);
            g.setColour(MyColours::blue);
            g.fillRoundedRectangle(bounds, 0.f);
        }
    
        
        
    }
    void setLevel(const float value)
    {
        level = value;
    }
    void resized() override
    {
        const auto bounds = getLocalBounds().toFloat();
        gradient = juce::ColourGradient{ juce::Colours::white, bounds.getBottomLeft(), MyColours::blue, bounds.getTopLeft(), false };
        gradient.addColour(0.5, MyColours::blue);
    }
    
    
private:
    float level = -60.f;
    juce::ColourGradient gradient{};


};

}
