/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DSP/RingBuffer.h"
#include "DSP/LinkwitzRileyFilters.h"
#include "DSP/DynamicsEngine.h"
#include "DSP/SingleChannelSampleFifo.h"






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
    
    float getRMSValue(const int channel) const;
    float getRMSPost(const int channel) const;
    float getRMSLb(const int channel) const;
    float getRMSUb(const int channel) const;
    juce::AudioProcessor::BusesProperties createBusesLayout();
    float processCore(float inputSample, float sideChain, int channel, juce::AudioBuffer<float>& lowerBandBuffer, juce::AudioBuffer<float>& upperBandBuffer, int sampleIndex);
    float processCore2(float inputSample, float sideChain, int channel, int sampleIndex);
    void updateRms(juce::AudioBuffer<float>& buffer, juce::LinearSmoothedValue<float>& rmsLevelLeft, juce::LinearSmoothedValue<float>& rmsLevelRight);
    void updateParams();
    
    juce::AudioProcessorValueTreeState parameters;
    
    using BlockType = juce::AudioBuffer<float>;
    SingleChannelSampleFifo<BlockType> leftChannelFifo { Channel::Left };
    SingleChannelSampleFifo<BlockType> rightChannelFifo { Channel::Right };
    
    

private:
    juce::AudioParameterFloat* inputGain   { nullptr };
    juce::AudioParameterFloat* makeupGain   { nullptr };
    juce::AudioParameterFloat* hbGain   { nullptr };
    juce::AudioParameterFloat* lbGain   { nullptr };
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
    juce::AudioParameterBool* compressionModep { nullptr };
    juce::AudioParameterBool* detectionMode { nullptr };
    
    float mInputGain, mMakeupGain, mHbGain, mLbGain, mMix, mDelay,mCrossoverFreq, mReleaseLb, mReleaseHb, mAttackLb, mAttackHb, mKneeLb, mKneeHb, mThresholdLb, mThresholdHb, mRatioLb, mRatioHb;
    
    bool mDelayOn, mFeedback, mSideChain, mCompressionMode, mDetectionMode;
    
    
    juce::LinearSmoothedValue<float> rmsLevelLeft, rmsLevelRight, rmsLevelLeftPost, rmsLevelRightPost, rmsLevelLeftLB, rmsLevelRightLB, rmsLevelLeftUB, rmsLevelRightUB, inputGainSmooth, makeupGainSmooth, lbGainSmooth, hbGainSmooth;
    juce::SmoothedValue <float, juce::WindowedSincInterpolator> delaySmooth2 {1.0f};
    
    int mSampleRate;
    std::vector<RingBuffer> rbufferVec;
    std::vector<LinkwitzRiley2ndOrderLowPass> lRLowPassVec;
    std::vector<LinkwitzRiley2ndOrderHighPass> lRHighPassVec;
    std::vector<DynamicsEngine> dynamicsVec;
    juce::AudioBuffer<float> lowerBandBuffer;
    juce::AudioBuffer<float> upperBandBuffer;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Assesment2AudioProcessor)
};
