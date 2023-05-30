/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DSP/RingBuffer.h"
#include "DSP/LinkwitzRileyFilters.h"
#include "DSP/ButterworthFilters.h"
#include "DSP/DynamicsEngine.h"
//==============================================================================
/**
*/
class Assesment2AudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    Assesment2AudioProcessor();
    ~Assesment2AudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    juce::AudioProcessor::BusesProperties createBusesLayout();
    float processCore(float inputSample, float sideChain, float channel);

private:
    juce::AudioProcessorValueTreeState parameters;
    juce::AudioParameterFloat* inputGain   { nullptr };
    juce::AudioParameterFloat* makeupGain   { nullptr };
    juce::AudioParameterFloat* mix  { nullptr };
    juce::AudioParameterFloat* delay  { nullptr };
    juce::AudioParameterFloat* crossoverFreq {nullptr};
    juce::AudioParameterFloat* releaseLb {nullptr};
    juce::AudioParameterFloat* releaseHb {nullptr};
    juce::AudioParameterFloat* attackLb {nullptr};
    juce::AudioParameterFloat* attackHb {nullptr};
    juce::AudioParameterFloat* kneeLb {nullptr};
    juce::AudioParameterFloat* kneeHb {nullptr};
    juce::AudioParameterFloat* thresholdLb {nullptr};
    juce::AudioParameterFloat* thresholdHb {nullptr};
    juce::AudioParameterFloat* ratioLb {nullptr};
    juce::AudioParameterFloat* ratioHb {nullptr};
    juce::AudioParameterBool* delayOn { nullptr };
    juce::AudioParameterBool* feedback { nullptr };
    juce::AudioParameterBool* sidechain { nullptr };
    juce::AudioParameterChoice* compressionModep { nullptr };
    juce::AudioParameterChoice* detectionMode { nullptr };
    juce::AudioParameterChoice* compressionSource { nullptr };
    
    
    
    int mSampleRate;
    std::vector<RingBuffer> rbufferVec;
    std::vector<LinkwitzRiley2ndOrderLowPass> lRLowPassVec;
    std::vector<LinkwitzRiley2ndOrderHighPass> lRHighPassVec;
    std::vector<LinkwitzRiley4thOrderLowPass> lR4LowPassVec;
    std::vector<LinkwitzRiley4thOrderHighPass> lR4HighPassVec;
    std::vector<ButterworthLowPass> ButterLowPassVec;
    std::vector<ButterworthHighPass> ButterHighPassVec;
    std::vector<DynamicsEngine> dynamicsVec;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Assesment2AudioProcessor)
};
