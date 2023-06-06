/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ParamsIDs.h"
#include "Utils.h"

static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    const auto ratioRange = juce::NormalisableRange<float> { 1.0f, 30.0f, 0.1f, 1.0f };
    const auto crossoverRange = juce::NormalisableRange<float> { 40.0f, 20000.0f, 1.f, 0.3f };
    const auto mixRange = juce::NormalisableRange<float> { 0.0f, 100.0f, 1.f, 1.0f };
    // Audio parameter floats
    // Multiband float Parameters
    layout.add(std::make_unique<juce::AudioParameterFloat> (juce::ParameterID(ParamIDs::attackLb, 1),
                                                            ParamIDs::attackLb,
                                                            0.01f,
                                                            100.0f,
                                                            30.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat> (juce::ParameterID(ParamIDs::attackHb, 1),
                                                            ParamIDs::attackHb,
                                                            0.01f,
                                                            100.0f,
                                                            30.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat> (juce::ParameterID(ParamIDs::releaseLb, 1),
                                                            ParamIDs::releaseLb,
                                                            5.0f,
                                                            3000.0f,
                                                            400.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat> (juce::ParameterID(ParamIDs::releaseHb, 1),
                                                            ParamIDs::releaseHb,
                                                            5.0f,
                                                            3000.0f,
                                                            400.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat> (juce::ParameterID(ParamIDs::thresholdLb, 1),
                                                            ParamIDs::thresholdLb,
                                                            -50.0f,
                                                            0.0f,
                                                            0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat> (juce::ParameterID(ParamIDs::thresholdHb, 1),
                                                            ParamIDs::thresholdHb,
                                                            -50.0f,
                                                            0.0f,
                                                            0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat> (juce::ParameterID(ParamIDs::ratioLb, 1),
                                                            ParamIDs::ratioLb,
                                                            ratioRange,
                                                            1.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat> (juce::ParameterID(ParamIDs::ratioHb, 1),
                                                            ParamIDs::ratioHb,
                                                            ratioRange,
                                                            1.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat> (juce::ParameterID(ParamIDs::kneeLb, 1),
                                                            ParamIDs::kneeLb,
                                                            0.0f,
                                                            1.0f,
                                                            0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat> (juce::ParameterID(ParamIDs::kneeHb, 1),
                                                            ParamIDs::kneeHb,
                                                            0.0f,
                                                            1.0f,
                                                            0.0f));
    // General float Parameters
    layout.add(std::make_unique<juce::AudioParameterFloat> (juce::ParameterID(ParamIDs::gain, 1),
                                                            ParamIDs::gain,
                                                            -12.0f,
                                                            24.0f,
                                                            0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat> (juce::ParameterID(ParamIDs::makeup, 1),
                                                            ParamIDs::makeup,
                                                            -12.0f,
                                                            24.0f,
                                                            0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat> (juce::ParameterID(ParamIDs::crossover, 1),
                                                            ParamIDs::crossover,
                                                            crossoverRange,
                                                            2000.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat> (juce::ParameterID(ParamIDs::delay, 1),
                                                            ParamIDs::delay,
                                                            1.0f,
                                                            20.0f,
                                                            1.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat> (juce::ParameterID(ParamIDs::mix, 1),
                                                            ParamIDs::mix,
                                                            mixRange,
                                                            100.0f));
    
   
    // Audio parameter choices
    layout.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID(ParamIDs::mode, 1),
                                                            ParamIDs::mode,
                                                            juce::StringArray({"Compressor", "Limiter"}),
                                                            0));
    layout.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID(ParamIDs::detection, 1),
                                                            ParamIDs::detection,
                                                            juce::StringArray({"Peak", "RMS"}),
                                                            0));
    
    // Audio parameter bools
    layout.add(std::make_unique<juce::AudioParameterBool> (juce::ParameterID(ParamIDs::delayButton, 1),
                                                           ParamIDs::delayButton,
                                                           false));
    layout.add(std::make_unique<juce::AudioParameterBool> (juce::ParameterID(ParamIDs::feedback, 1),
                                                           ParamIDs::feedback,
                                                           false));
    layout.add(std::make_unique<juce::AudioParameterBool> (juce::ParameterID(ParamIDs::source, 1),
                                                           ParamIDs::source,
                                                           false));

    
    return layout;
}



//==============================================================================
Assesment2AudioProcessor::Assesment2AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (createBusesLayout()),
                        parameters(*this, nullptr, "Parameters", createParameterLayout())
#endif
{
    auto storeFloatParam = [&parameters = this->parameters](auto& param, const auto& paramID)
    {
        param = dynamic_cast<juce::AudioParameterFloat*> (parameters.getParameter (paramID));
        jassert (param != nullptr);
    };
    
    storeFloatParam (inputGain,  ParamIDs::gain);
    storeFloatParam (makeupGain, ParamIDs::makeup);
    storeFloatParam(mix, ParamIDs::mix);
    storeFloatParam(delay, ParamIDs::delay);
    storeFloatParam(crossoverFreq, ParamIDs::crossover);
    storeFloatParam(releaseLb, ParamIDs::releaseLb);
    storeFloatParam(releaseHb, ParamIDs::releaseHb);
    storeFloatParam(attackLb, ParamIDs::attackLb);
    storeFloatParam(attackHb, ParamIDs::attackHb);
    storeFloatParam(kneeLb, ParamIDs::kneeLb);
    storeFloatParam(kneeHb, ParamIDs::kneeHb);
    storeFloatParam(thresholdLb, ParamIDs::thresholdLb);
    storeFloatParam(thresholdHb, ParamIDs::thresholdHb);
    storeFloatParam(ratioLb, ParamIDs::ratioLb);
    storeFloatParam(ratioHb, ParamIDs::ratioHb);
    
    auto storeBoolParam = [&parameters = this->parameters](auto& param, const auto& paramID)
    {
        param = dynamic_cast<juce::AudioParameterBool*> (parameters.getParameter (paramID));
        jassert (param != nullptr);
    };
    
    storeBoolParam(delayOn, ParamIDs::delayButton);
    storeBoolParam(feedback, ParamIDs::feedback);
    storeBoolParam(sidechain, ParamIDs::source);
    
    auto storeChoiceParam = [&parameters = this->parameters](auto& param, const auto& paramID)
    {
        param = dynamic_cast<juce::AudioParameterChoice*> (parameters.getParameter (paramID));
        jassert (param != nullptr);
    };
    
    storeChoiceParam(compressionModep, ParamIDs::mode);
    storeChoiceParam(detectionMode, ParamIDs::detection);
    
    
    
}

Assesment2AudioProcessor::~Assesment2AudioProcessor()
{
}

//==============================================================================
const juce::String Assesment2AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Assesment2AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Assesment2AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Assesment2AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Assesment2AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Assesment2AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Assesment2AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Assesment2AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Assesment2AudioProcessor::getProgramName (int index)
{
    return {};
}

void Assesment2AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Assesment2AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    const int totalNumInputChannels  = getTotalNumInputChannels();
    mSampleRate = sampleRate;
    int maxDelay = sampleRate * 0.03;
    rbufferVec.clear();
    lRLowPassVec.clear();
    lRHighPassVec.clear();
    ButterLowPassVec.clear();
    ButterHighPassVec.clear();
    lR4LowPassVec.clear();
    lR4HighPassVec.clear();
    dynamicsVec.clear();
    for (int i = 0; i < totalNumInputChannels; i++)
    {
        rbufferVec.push_back(RingBuffer(maxDelay + 3));
        lRLowPassVec.push_back(LinkwitzRiley2ndOrderLowPass(crossoverFreq->get(), sampleRate));
        lRLowPassVec.push_back(LinkwitzRiley2ndOrderLowPass(crossoverFreq->get(), sampleRate));
        lRHighPassVec.push_back(LinkwitzRiley2ndOrderHighPass(crossoverFreq->get(), sampleRate));
        lRHighPassVec.push_back(LinkwitzRiley2ndOrderHighPass(crossoverFreq->get(), sampleRate));
        lR4LowPassVec.push_back(LinkwitzRiley4thOrderLowPass(crossoverFreq->get(), sampleRate));
        lR4HighPassVec.push_back(LinkwitzRiley4thOrderHighPass(crossoverFreq->get(), sampleRate));
        dynamicsVec.push_back(DynamicsEngine(sampleRate));
        dynamicsVec.push_back(DynamicsEngine(sampleRate));
    }
    
    rmsLevelLeft.reset(sampleRate, 0.5);
    rmsLevelRight.reset(sampleRate, 0.5);
    rmsLevelLeftPost.reset(sampleRate, 0.5);
    rmsLevelRightPost.reset(sampleRate, 0.5);
    
    rmsLevelLeft.setCurrentAndTargetValue(-100.f);
    rmsLevelRight.setCurrentAndTargetValue(-100.f);
    rmsLevelLeftPost.setCurrentAndTargetValue(-100.f);
    rmsLevelRightPost.setCurrentAndTargetValue(-100.f);
    
}

void Assesment2AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Assesment2AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Assesment2AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
//     Check if a sidechain input is available
    if (getTotalNumInputChannels() > getMainBusNumInputChannels() && sidechain->get())
    {
        juce::AudioBuffer<float> mainInputOutput = getBusBuffer (buffer, true, 0);
        juce::AudioBuffer<float> sideChainInput = getBusBuffer (buffer, true, 1);

        for (int channel = 0; channel < sideChainInput.getNumChannels(); ++channel)
        {
            const float* input = mainInputOutput.getReadPointer (channel);
            const float* side = sideChainInput.getReadPointer (channel);
            float* output = mainInputOutput.getWritePointer (channel);

            assert(mainInputOutput.getNumSamples() == sideChainInput.getNumSamples());

            for (int sample = 0; sample < mainInputOutput.getNumSamples(); ++sample)
            {
                float inputSample = input[sample];
                float sideChainSample = side[sample];
                output[sample] = processCore(inputSample, sideChainSample, channel);
            }
        }
    }
    else
    {
        rmsLevelLeft.skip(buffer.getNumSamples());
        rmsLevelRight.skip(buffer.getNumSamples());
        {
        const auto value = juce::Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, buffer.getNumSamples()));
        if (value < rmsLevelLeft.getCurrentValue())
            rmsLevelLeft.setTargetValue(value);
        else
            rmsLevelLeft.setCurrentAndTargetValue(value);
        }
        {
        const auto value = juce::Decibels::gainToDecibels(buffer.getRMSLevel(1, 0, buffer.getNumSamples()));
        if (value < rmsLevelRight.getCurrentValue())
            rmsLevelRight.setTargetValue(value);
        else
            rmsLevelRight.setCurrentAndTargetValue(value);
        }
        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {


            auto* input = buffer.getReadPointer ( channel );
            auto* output = buffer.getWritePointer (channel);

            for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            {
                float inputSample = input[sample];
                output[sample] = processCore(inputSample, inputSample, channel);
            }
        }
    }
    
    rmsLevelLeftPost.skip(buffer.getNumSamples());
    rmsLevelRightPost.skip(buffer.getNumSamples());
    {
    const auto value = juce::Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, buffer.getNumSamples()));
    if (value < rmsLevelLeftPost.getCurrentValue())
        rmsLevelLeftPost.setTargetValue(value);
    else
        rmsLevelLeftPost.setCurrentAndTargetValue(value);
    }
    {
    const auto value = juce::Decibels::gainToDecibels(buffer.getRMSLevel(1, 0, buffer.getNumSamples()));
    if (value < rmsLevelRightPost.getCurrentValue())
        rmsLevelRightPost.setTargetValue(value);
    else
        rmsLevelRightPost.setCurrentAndTargetValue(value);
    }
    
    
    
}

//==============================================================================
bool Assesment2AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Assesment2AudioProcessor::createEditor()
{
//    return new juce::GenericAudioProcessorEditor (*this);
    return new Assesment2AudioProcessorEditor (*this);
}

//==============================================================================
void Assesment2AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void Assesment2AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
    {
        if (xmlState->hasTagName (parameters.state.getType()))
        {
            parameters.replaceState (juce::ValueTree::fromXml (*xmlState));
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Assesment2AudioProcessor();
}



juce::AudioProcessor::BusesProperties Assesment2AudioProcessor::createBusesLayout()
{
    BusesProperties bp = BusesProperties().withInput ("Input", juce::AudioChannelSet::stereo(), true)
    .withOutput ("Output", juce::AudioChannelSet::stereo(), true);
    
    if (!juce::JUCEApplicationBase::isStandaloneApp())
    {// not working in standalone
        bp = bp.withInput ("SideChain", juce::AudioChannelSet::stereo(), true);
    }
    
    return bp;
    
}



float Assesment2AudioProcessor::processCore(float inputSample, float sideChainInput, float channel)
{
    
    
    float processingSample = inputSample;

    //Look-ahead delay processing
    if(delayOn->get())
    {
        float delaySamples = (delay->get()/1000) * mSampleRate;
        processingSample = rbufferVec[channel].readInterp(delaySamples);
    }
    rbufferVec[channel].write(inputSample);

    //Update Linwitz Riley filters
    lRLowPassVec[channel].updateCoefs(crossoverFreq->get());
    lRHighPassVec[channel].updateCoefs(crossoverFreq->get());
    // Split into bands
    float lowerBand = lRLowPassVec[channel].process(processingSample);
    float upperBand = lRHighPassVec[channel].process(processingSample);
    // Apply input gain
    lowerBand = lowerBand * juce::Decibels::decibelsToGain(inputGain->get()) * 0.75;
    upperBand = upperBand * juce::Decibels::decibelsToGain(inputGain->get()) * 0.75;
    float joinedInputBands = lowerBand + upperBand * -1;
    // Cook parameters for Low band
    float attackTimeSeconds = attackLb->get() / 1000;
    float releaseTimeSeconds = releaseLb->get() / 1000;
    float kneeDb = kneeLb->get() * 20;
    // Set Dynamics processor for low band
    dynamicsVec[channel].setParameters(compressionModep->getIndex(), feedback->get(), detectionMode->getIndex(), thresholdLb->get(), attackTimeSeconds, releaseTimeSeconds, kneeDb, ratioLb->get());
    // Cook parameters for High band
    attackTimeSeconds = attackHb->get() / 1000;
    releaseTimeSeconds = releaseHb->get() / 1000;
    kneeDb = kneeHb->get() * 20;
    // Set Dynamics processor for high band
    dynamicsVec[channel+2].setParameters(compressionModep->getIndex(), feedback->get(), detectionMode->getIndex(), thresholdHb->get(), attackTimeSeconds, releaseTimeSeconds, kneeDb, ratioHb->get());
    // Process Dynamics processor
    float outputSampleLb = dynamicsVec[channel].process(lowerBand, sideChainInput);
    float outputSampleHb = dynamicsVec[channel+2].process(upperBand, sideChainInput);
    // Add seperate bands back together
    float outputSample = outputSampleLb + outputSampleHb  * -1;
    //Apply output gain
    outputSample = outputSample * juce::Decibels::decibelsToGain(makeupGain->get());
    outputSample = outputSample * juce::Decibels::decibelsToGain(makeupGain->get());
    float processedMix = mix->get() / 100;
    return (1-processedMix) * joinedInputBands + processedMix * outputSample;
    
    
    
}


float Assesment2AudioProcessor::getRMSValue(const int channel) const
{
    jassert(channel == 0 || channel == 1);
    if (channel == 0)
        return rmsLevelLeft.getCurrentValue();
    if (channel == 1)
        return rmsLevelRight.getCurrentValue();
    return 0.f;
}


float Assesment2AudioProcessor::getRMSPost(const int channel) const
{
    jassert(channel == 0 || channel == 1);
    if (channel == 0)
        return rmsLevelLeftPost.getCurrentValue();
    if (channel == 1)
        return rmsLevelRightPost.getCurrentValue();
    return 0.f;
}
