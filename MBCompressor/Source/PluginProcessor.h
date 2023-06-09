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
#include <array>

template<typename T>
struct Fifo
{
    void prepare(int numChannels, int numSamples)
    {
        static_assert( std::is_same_v<T, juce::AudioBuffer<float>>,
                      "prepare(numChannels, numSamples) should only be used when the Fifo is holding juce::AudioBuffer<float>");
        for( auto& buffer : buffers)
        {
            buffer.setSize(numChannels,
                           numSamples,
                           false,   //clear everything?
                           true,    //including the extra space?
                           true);   //avoid reallocating if you can?
            buffer.clear();
        }
    }
    
    void prepare(size_t numElements)
    {
        static_assert( std::is_same_v<T, std::vector<float>>,
                      "prepare(numElements) should only be used when the Fifo is holding std::vector<float>");
        for( auto& buffer : buffers )
        {
            buffer.clear();
            buffer.resize(numElements, 0);
        }
    }
    
    bool push(const T& t)
    {
        auto write = fifo.write(1);
        if( write.blockSize1 > 0 )
        {
            buffers[write.startIndex1] = t;
            return true;
        }
        
        return false;
    }
    
    bool pull(T& t)
    {
        auto read = fifo.read(1);
        if( read.blockSize1 > 0 )
        {
            t = buffers[read.startIndex1];
            return true;
        }
        
        return false;
    }
    
    int getNumAvailableForReading() const
    {
        return fifo.getNumReady();
    }
private:
    static constexpr int Capacity = 30;
    std::array<T, Capacity> buffers;
    juce::AbstractFifo fifo {Capacity};
};



enum Channel
{
    Right, //effectively 0
    Left //effectively 1
};

template<typename BlockType>
struct SingleChannelSampleFifo
{
    SingleChannelSampleFifo(Channel ch) : channelToUse(ch)
    {
        prepared.set(false);
    }
    
    void update(const BlockType& buffer)
    {
        jassert(prepared.get());
        jassert(buffer.getNumChannels() > channelToUse );
        auto* channelPtr = buffer.getReadPointer(channelToUse);
        
        for( int i = 0; i < buffer.getNumSamples(); ++i )
        {
            pushNextSampleIntoFifo(channelPtr[i]);
        }
    }

    void prepare(int bufferSize)
    {
        prepared.set(false);
        size.set(bufferSize);
        
        bufferToFill.setSize(1,             //channel
                             bufferSize,    //num samples
                             false,         //keepExistingContent
                             true,          //clear extra space
                             true);         //avoid reallocating
        audioBufferFifo.prepare(1, bufferSize);
        fifoIndex = 0;
        prepared.set(true);
    }
    //==============================================================================
    int getNumCompleteBuffersAvailable() const { return audioBufferFifo.getNumAvailableForReading(); }
    bool isPrepared() const { return prepared.get(); }
    int getSize() const { return size.get(); }
    //==============================================================================
    bool getAudioBuffer(BlockType& buf) { return audioBufferFifo.pull(buf); }
private:
    Channel channelToUse;
    int fifoIndex = 0;
    Fifo<BlockType> audioBufferFifo;
    BlockType bufferToFill;
    juce::Atomic<bool> prepared = false;
    juce::Atomic<int> size = 0;
    
    void pushNextSampleIntoFifo(float sample)
    {
        if (fifoIndex == bufferToFill.getNumSamples())
        {
            auto ok = audioBufferFifo.push(bufferToFill);

            juce::ignoreUnused(ok);
            
            fifoIndex = 0;
        }
        
        bufferToFill.setSample(0, fifoIndex, sample);
        ++fifoIndex;
    }
};




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
    void updateRms(juce::AudioBuffer<float>& buffer, juce::LinearSmoothedValue<float>& rmsLevelLeft, juce::LinearSmoothedValue<float>& rmsLevelRight);
    
    
    juce::AudioProcessorValueTreeState parameters;

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
    juce::AudioParameterChoice* compressionSource { nullptr };
    
    juce::LinearSmoothedValue<float> rmsLevelLeft, rmsLevelRight, rmsLevelLeftPost, rmsLevelRightPost, rmsLevelLeftLB, rmsLevelRightLB, rmsLevelLeftUB, rmsLevelRightUB;
    
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
