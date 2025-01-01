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

class VerticalMeter : public juce::Component
{
public:
    void paint(juce::Graphics& g) override
    {
        auto bounds = getLocalBounds().toFloat().reduced(3.f);

        g.setColour(juce::Colours::white.withBrightness(0.4f));
        g.fillRoundedRectangle(bounds, 0.f);
        g.setColour(juce::Colours::white);
        const auto scaledY = juce::jmap(level, -60.f,6.f,0.f,static_cast<float>(getHeight()));
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

    
private:
    float level = -60.f;
    juce::ColourGradient gradient{};


};









}
