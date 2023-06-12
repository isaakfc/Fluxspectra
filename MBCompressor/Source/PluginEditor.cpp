/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"






void PathProducer::process(juce::Rectangle<float> fftBounds, double sampleRate)
{
    juce::AudioBuffer<float> tempIncomingBuffer;
    while( leftChannelFifo->getNumCompleteBuffersAvailable() > 0 )
    {
        if( leftChannelFifo->getAudioBuffer(tempIncomingBuffer) )
        {
            auto size = tempIncomingBuffer.getNumSamples();

            juce::FloatVectorOperations::copy(monoBuffer.getWritePointer(0, 0),
                                              monoBuffer.getReadPointer(0, size),
                                              monoBuffer.getNumSamples() - size);

            juce::FloatVectorOperations::copy(monoBuffer.getWritePointer(0, monoBuffer.getNumSamples() - size),
                                              tempIncomingBuffer.getReadPointer(0, 0),
                                              size);
            
            leftChannelFFTDataGenerator.produceFFTDataForRendering(monoBuffer, -48.f);
        }
    }
    
    const auto fftSize = leftChannelFFTDataGenerator.getFFTSize();
    const auto binWidth = sampleRate / double(fftSize);

    while( leftChannelFFTDataGenerator.getNumAvailableFFTDataBlocks() > 0 )
    {
        std::vector<float> fftData;
        if( leftChannelFFTDataGenerator.getFFTData( fftData) )
        {
            pathProducer.generatePath(fftData, fftBounds, fftSize, binWidth, -48.f);
        }
    }
    
    while( pathProducer.getNumPathsAvailable() > 0 )
    {
        pathProducer.getPath( leftChannelFFTPath );
    }
}

void SpectrumAnalyser::timerCallback()
{
    if( shouldShowFFTAnalysis )
    {
        auto fftBounds = getAnalysisArea().toFloat();
        auto sampleRate = audioProcessor.getSampleRate();
        
        leftPathProducer.process(fftBounds, sampleRate);
        rightPathProducer.process(fftBounds, sampleRate);
    }

    if( parametersChanged.compareAndSetBool(false, true) )
    {

    }
    
    repaint();
}



juce::Rectangle<int> SpectrumAnalyser::getRenderArea()
{
    auto bounds = getLocalBounds();
    
    bounds.removeFromTop(12);
    bounds.removeFromBottom(2);
    bounds.removeFromLeft(20);
    bounds.removeFromRight(20);
    
    return bounds;
}


juce::Rectangle<int> SpectrumAnalyser::getAnalysisArea()
{
    auto bounds = getRenderArea();
    bounds.removeFromTop(4);
    bounds.removeFromBottom(4);
    return bounds;
}
//==============================================================================


//==============================================================================
Assesment2AudioProcessorEditor::Assesment2AudioProcessorEditor (Assesment2AudioProcessor& p)
: AudioProcessorEditor (&p), topBand(MyColours::black), audioProcessor (p), leftBand(MyColours::black, p), rightBand(MyColours::black, p),spectrogramBand(p), bottomBand(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    juce::LookAndFeel::setDefaultLookAndFeel(&myCustomLNF);
    
    addAndMakeVisible(topBand);
    addAndMakeVisible(globalBand);
    addAndMakeVisible(rightBand);
    addAndMakeVisible(leftBand);
    addAndMakeVisible(bottomBand);
    addAndMakeVisible(spectrogramBand);
    addAndMakeVisible(analyzer);
    
    // Custom Slider Code
//    mySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
//    mySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 15);
//    mySlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentWhite);
//    mySlider.setColour(juce::Slider::textBoxTextColourId,  MyColours::logotext);
//    juce::Font fontbold (juce::Font (juce::Typeface::createSystemTypefaceFor (BinaryData::LexendPetaBold_ttf,
//                                                                            BinaryData::LexendPetaBold_ttfSize)));
//    fontbold.setHeight(20);
//    mySliderLabel.setFont(fontbold);
//    mySliderLabel.setColour(juce::Label::textColourId, MyColours::logotext);
//    mySliderLabel.setText("CROSSOVER", juce::dontSendNotification);
//
    
    
//    addAndMakeVisible(mySliderLabel);
//    addAndMakeVisible(mySlider);
    addAndMakeVisible(logoAndText);
    
    setSize (720, 450);
}

Assesment2AudioProcessorEditor::~Assesment2AudioProcessorEditor()
{
}

//==============================================================================
void Assesment2AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    

    

}

void Assesment2AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto bounds = getLocalBounds();
    
    topBand.setBounds( bounds.removeFromTop(90) );
    leftBand.setBounds(bounds.removeFromLeft(30));
    rightBand.setBounds(bounds.removeFromRight(30));
    globalBand.setBounds(bounds.removeFromRight(150));
    bottomBand.setBounds(bounds.removeFromBottom(130));
    spectrogramBand.setBounds(bounds);
    bounds.reduce(5, 5);
    analyzer.setBounds(bounds);
    
    
    bounds = getLocalBounds();
    mySlider.setBounds(bounds.getWidth() / 1.375 , bounds.getHeight() / 2 + 25, 200, 180);
    mySliderLabel.setBounds(mySlider.getX() + mySlider.getWidth() / 4.8, mySlider.getY() + mySlider.getHeight() / 1.7 , 120, 100);
    logoAndText.setBounds(topBand.getBounds());
}



