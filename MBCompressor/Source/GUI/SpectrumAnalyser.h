/*
  ==============================================================================

    SpectrumAnalyser.h
    Created: 9 Jun 2023 3:04:26pm
    Author:  Isaac Faraz-Chandler

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PathProducer.h"

struct SpectrumAnalyser: juce::Component,
juce::AudioProcessorParameter::Listener,
juce::Timer
{
    SpectrumAnalyser(Assesment2AudioProcessor&);
    ~SpectrumAnalyser();
    
    void parameterValueChanged (int parameterIndex, float newValue) override;

    void parameterGestureChanged (int parameterIndex, bool gestureIsStarting) override { }
    
    void timerCallback() override;
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    
    void toggleAnalysisEnablement(bool enabled)
    {
        shouldShowFFTAnalysis = enabled;
    }
private:
    Assesment2AudioProcessor& audioProcessor;

    bool shouldShowFFTAnalysis = true;

    juce::Atomic<bool> parametersChanged { false };
    
    void drawBackgroundGrid(juce::Graphics& g);
    void drawTextLabels(juce::Graphics& g);
    
    std::vector<float> getFrequencies();
    std::vector<float> getGains();
    std::vector<float> getXs(const std::vector<float>& freqs, float left, float width);

    juce::Rectangle<int> getRenderArea();
    
    juce::Rectangle<int> getAnalysisArea();
    
    PathProducer leftPathProducer, rightPathProducer;
};
