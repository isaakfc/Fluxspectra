/*
  ==============================================================================

    LogoAndTitle.h
    Created: 3 Jun 2023 10:29:34pm
    Author:  Isaac Faraz-Chandler

  ==============================================================================
*/

#pragma once

class LogoAndTextComponent : public juce::Component
{
public:
    void paint (juce::Graphics& g) override
    {
        juce::Typeface::Ptr font1 = juce::Typeface::createSystemTypefaceFor(BinaryData::LexendPetaRegular_ttf, BinaryData::LexendPetaRegular_ttfSize);
        
        g.setFont(font1);
        g.setFont(16.0f);
        g.setColour(MyColours::logotext);
        g.drawFittedText("SEMBLANCE", 42, 55, 100, 50, juce::Justification::centred, 1);
        juce::Typeface::Ptr font2 = juce::Typeface::createSystemTypefaceFor(BinaryData::LexendPetaExtraBold_ttf, BinaryData::LexendPetaExtraBold_ttfSize);
    //    theFont = juce::Font("Lexend Peta", "ExtraBold", 40.0f);
        g.setFont(font2);
        g.setFont(70.0f);
        g.drawFittedText("FLUXSPECTRA", 144, 0, 530, 100, juce::Justification::centred, 1);
        logo = juce::ImageCache::getFromMemory(BinaryData::iflogonew_png, BinaryData::iflogonew_pngSize);
        g.drawImageWithin(logo, 54, 0, 75, 75, juce::RectanglePlacement::stretchToFit);
//        juce::Font font ("Helvetica", 20.0f, juce::Font::plain);
        juce::Font font3 ("Helvetica", 20.0f, juce::Font::plain);
        g.setFont(font3);
        g.setFont(13.f);
        int startX = 7;
        int startY = 10;
        int yIncrement = 20;
        g.drawFittedText("G", startX, startY, 20, 20, juce::Justification::centred, 1);
        g.drawFittedText("A", startX, startY + yIncrement, 20, 20, juce::Justification::centred, 1);
        g.drawFittedText("I", startX, startY + yIncrement * 2, 20, 20, juce::Justification::centred, 1);
        g.drawFittedText("N", startX, startY + yIncrement * 3, 20, 20, juce::Justification::centred, 1);
        startX = getWidth() - 25;
        yIncrement = 13;
        g.drawFittedText("M", startX, startY, 20, 20, juce::Justification::centred, 1);
        g.drawFittedText("A", startX, startY + yIncrement, 20, 20, juce::Justification::centred, 1);
        g.drawFittedText("K", startX, startY + yIncrement * 2, 20, 20, juce::Justification::centred, 1);
        g.drawFittedText("E", startX, startY + yIncrement * 3, 20, 20, juce::Justification::centred, 1);
        g.drawFittedText("U", startX, startY + yIncrement * 4, 20, 20, juce::Justification::centred, 1);
        g.drawFittedText("P", startX, startY + yIncrement * 5, 20, 20, juce::Justification::centred, 1);
        
        
    }
private:
        juce::Image logo;
};
