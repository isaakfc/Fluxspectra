/*
  ==============================================================================

    CustomUI.cpp
    Created: 31 May 2023 1:59:28pm
    Author:  Isaac Faraz-Chandler

  ==============================================================================
*/

// Include the custom UI header file.
#include "CustomUI.h"

const juce::Font& getFont()
{
    static juce::Font font (juce::Font (juce::Typeface::createSystemTypefaceFor (BinaryData::LexendPetaBold_ttf,
                                                                BinaryData::LexendPetaBold_ttfSize)));
    return font;
}


// Use the juce namespace.
namespace juce
{
    // Constructor for CustomDial2 class. This is where you can initialize your class variables.
    CustomDial2::CustomDial2()
    {
    
    }

    // This function is responsible for the appearance of the slider.
    // It's called each time the GUI needs to be updated (e.g., when the slider moves).
    void CustomDial2::drawRotarySlider
    (juce::Graphics &g,int x,int y,int width,int height,float sliderPos,float rotaryStartAngle,float rotaryEndAngle,juce::Slider &slider)
    {
        // These colors are retrieved from the Slider and can be customized elsewhere in your code.
        const auto outlineColor  = MyColours::logotext;
        const auto fillColor     = MyColours::blue;
        const auto mainColor     = MyColours::grey;
        const auto brighterColor = MyColours::background;
        const auto dialOutlineColor = MyColours::blackGrey;

        // Define the area where the dial will be drawn.
        auto dialBounds = juce::Rectangle<int> (x, y, width, height).toFloat();
        auto centre = dialBounds.getCentre();
        auto fullRadius = juce::jmin (dialBounds.getWidth() / 2.0f, dialBounds.getHeight() / 2.0f);
        
        // Set the color for the dots that will be drawn around the dial.
        g.setColour (MyColours::logotext);

        // Loop that draws the dots around the dial.
        for (int i = 0; i < 11; ++i)
        {
            // dotSize determines the size of the dots. It is calculated as a proportion of the slider's width.
            auto dotSize = width * 0.025;

            // The angle at which the dot will be drawn. The angle is proportional to the i value.
            const auto angle = juce::jmap (i / 10.0f, rotaryStartAngle, rotaryEndAngle);

            // The position at which the dot will be drawn. It is located on the circumference of a circle around the slider's center.
            const auto point = centre.getPointOnCircumference (fullRadius - width * 0.06f, angle);

            // Draw the dot as a small ellipse.
            g.fillEllipse (point.getX() - 3, point.getY() - 3, dotSize, dotSize);
        }

        // Adjust the fullRadius variable for future calculations.
        fullRadius -= width / 14.5;

        // The angle to which the slider is currently set.
        auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        // Determine the width of the slider's track.
        float lineWidthMultiplier = width * 0.035;
        auto lineWidth = juce::jmin (lineWidthMultiplier, fullRadius * 0.5f);

        // Calculate the radius of the arc that represents the slider's track.
        auto arcRadius  = fullRadius - lineWidth * 2.25;

        // Define a Path object that will be used to draw the slider's track.
        juce::Path backgroundArc;
        backgroundArc.addCentredArc
        (
            dialBounds.getCentreX(),
            dialBounds.getCentreY(),
            arcRadius,
            arcRadius,
            0.0f,
            rotaryStartAngle,
            rotaryEndAngle,
            true
         );

        // Set the color for the slider's track and draw it.
        g.setColour (outlineColor);
        g.strokePath (backgroundArc, juce::PathStrokeType (lineWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        // Determine the radius of the dial.
        auto dialRadius = std:: max (fullRadius - 4.0f * lineWidth, 10.0f);

        // Create a scope in which certain graphic settings are saved.
        {
            juce::Graphics::ScopedSaveState saved (g);

            // If the slider is enabled, create a gradient fill and apply it to the dial.
            if (slider.isEnabled())
            {
                juce::ColourGradient fillGradient
                (
                    brighterColor,
                    centre.getX() + lineWidth * 0.9f,
                    centre.getY() - lineWidth * 10.0f,
                    mainColor.darker(1.0),
                    centre.getX() + dialRadius,
                    centre.getY() + dialRadius,
                    true
                );

                g.setGradientFill (fillGradient);
            }
        
            // Draw the dial.
            g.fillEllipse (centre.getX() - dialRadius, centre.getY() - dialRadius, dialRadius * 2.0f, dialRadius * 2.0f);
        }

        // Set the color for the dial outline and draw it.
        g.setColour (dialOutlineColor);
        auto scale = 2.0f;
        g.drawEllipse (centre.getX() - dialRadius, centre.getY() - dialRadius, dialRadius * scale, dialRadius * scale, 4.5f);

        // Create a path for the value track of the dial.
        juce::Path dialValueTrack;
        dialValueTrack.addCentredArc
        (
            dialBounds.getCentreX(),
            dialBounds.getCentreY(),
            arcRadius,
            arcRadius,
            0.0f,
            rotaryStartAngle,
            toAngle,
            true
         );

        // Set the color for the value track fill and draw it.
        g.setColour (fillColor);
        g.strokePath (dialValueTrack, juce::PathStrokeType (lineWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        // Set the color for the dial tick and define its path.
        g.setColour (dialOutlineColor);
        juce::Path dialTick;
        dialTick.startNewSubPath (centre.getPointOnCircumference (dialRadius - lineWidth, toAngle));
        dialTick.lineTo (centre.getPointOnCircumference ((dialRadius - lineWidth) * 0.6f, toAngle));

        // Set the thickness for the dial tick and draw it.
        g.strokePath (dialTick, juce::PathStrokeType (lineWidth * 0.75, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        // Store the slider's width for use in other functions.
        _sliderWidth = slider.getWidth();
        
//        juce::Font font2 (juce::Font (juce::Typeface::createSystemTypefaceFor (BinaryData::LexendPetaBold_ttf,
//                                                                                BinaryData::LexendPetaBold_ttfSize)));
        g.setColour (MyColours::black);
        g.setFont (getFont());
        g.setFont(18.0f);
    }


void CustomDial2::drawLabel(juce::Graphics& g, juce::Label& label) 
    {
        g.fillAll(label.findColour(juce::Label::backgroundColourId));

        if (! label.isBeingEdited())
        {
            auto alpha = label.isEnabled() ? 1.0f : 0.5f;
            const juce::Font font(getLabelFont(label));

            g.setColour(label.findColour(juce::Label::textColourId).withMultipliedAlpha(alpha));
            g.setFont(font);

            juce::Rectangle<int> textArea(label.getBorderSize().subtractedFrom(label.getLocalBounds()));
            
            juce::Font font2 (juce::Font (juce::Typeface::createSystemTypefaceFor (BinaryData::LexendPetaBold_ttf,
                                                                                    BinaryData::LexendPetaBold_ttfSize)));
            g.setColour (MyColours::black);
            g.setFont (font2);
            g.setFont(18.0f);

            g.drawFittedText(label.getText(), textArea, label.getJustificationType(),
                             jmax(1, (int) (textArea.getHeight() / font.getHeight())),
                             label.getMinimumHorizontalScale());

            g.setColour(label.findColour(juce::Label::outlineColourId).withMultipliedAlpha(alpha));
        }
        else if (label.isEnabled())
        {
            g.setColour(label.findColour(juce::Label::outlineColourId));
        }
    
    
    
    
    }




CustomFader::CustomFader()
    {
        
    }

    void CustomFader::drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height,
                                           float sliderPos,
                                           float minSliderPos,
                                           float maxSliderPos,
                                           const juce::Slider::SliderStyle style, juce::Slider& slider)
    {
        _sliderWidth = width;
        
        auto isThreeVal = (style == juce::Slider::SliderStyle::ThreeValueVertical || style == juce::Slider::SliderStyle::ThreeValueHorizontal);

        auto trackWidth = juce::jmin (6.0f, slider.isHorizontal() ? (float) height * 0.25f : (float) width * 0.25f);

        juce::Point<float> startPoint (slider.isHorizontal() ? (float) x : (float) x + (float) width * 0.5f,
                                 slider.isHorizontal() ? (float) y + (float) height * 0.5f : (float) (height + y));

        juce::Point<float> endPoint (slider.isHorizontal() ? (float) (width + x) : startPoint.x,
                               slider.isHorizontal() ? startPoint.y : (float) y);

        juce::Path backgroundTrack;
        backgroundTrack.startNewSubPath (startPoint);
        backgroundTrack.lineTo (endPoint);
        g.setColour (MyColours::blackGrey);
        g.strokePath (backgroundTrack, { trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded });

        juce::Path valueTrack;
        juce::Point<float> minPoint, maxPoint, thumbPoint;

        auto kx = slider.isHorizontal() ? sliderPos : ((float) x + (float) width * 0.5f);
        auto ky = slider.isHorizontal() ? ((float) y + (float) height * 0.5f) : sliderPos;

        minPoint = startPoint;
        maxPoint = { kx, ky };
        
        valueTrack.startNewSubPath (minPoint);
        valueTrack.lineTo (isThreeVal ? thumbPoint : maxPoint);
        g.setColour (MyColours::blue);
        g.strokePath (valueTrack, { trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded });

        auto thumbWidth = width * 0.45f;
        
        // Create a path for the thumb
        juce::Path thumbPath;
        thumbPath.addRoundedRectangle (-thumbWidth * 0.5f, -thumbWidth * 0.25f, thumbWidth, thumbWidth * 0.35, 2.0f);

        // Calculate the bounds of the thumb
        auto thumbBounds = thumbPath.getBounds().toFloat();
        thumbBounds.setCentre (maxPoint);

        // Fill the thumb path with a solid color
        auto thumbColor = slider.findColour (juce::Slider::thumbColourId);
        g.setColour (MyColours::logotext);
        g.fillPath (thumbPath, juce::AffineTransform::translation(maxPoint.x, maxPoint.y));

        // Apply a gradient fill to the thumb path with a small height offset
        g.setGradientFill(juce::ColourGradient::vertical(juce::Colours::white.darker(0.4),
                                                         thumbBounds.getY() - 6.0f,
                                                         MyColours::logotext,
                                                         thumbBounds.getBottom() + 1.0f));
        g.fillPath (thumbPath, juce::AffineTransform::translation(maxPoint.x, maxPoint.y));

    }

    void CustomFader::drawLabel (juce::Graphics& g, juce::Label& label)
    {
        g.fillAll (label.findColour (juce::Label::backgroundColourId));

        label.setEditable(true);
        
        if (! label.isBeingEdited())
        {
            auto alpha = label.isEnabled() ? 1.0f : 0.5f;
            const juce::Font font (juce::Font ("Helvetica", _sliderWidth * 0.1, juce::Font::FontStyleFlags::bold));

            g.setColour (label.findColour (juce::Label::textColourId).withMultipliedAlpha (alpha));
            g.setFont (font);

            auto textArea = getLabelBorderSize (label).subtractedFrom (label.getLocalBounds());

            g.drawFittedText (label.getText(), textArea, label.getJustificationType(),
                              juce::jmax (1, (int) ((float) textArea.getHeight() / font.getHeight())),
                              label.getMinimumHorizontalScale());

            g.setColour (label.findColour (juce::Label::outlineColourId).withMultipliedAlpha (alpha));
        }
        
        else if (label.isEnabled())
        {
            g.setColour (label.findColour (juce::Label::outlineColourId));
        }

        g.drawRect (label.getLocalBounds());
    }

    
        CustomTextButton::CustomTextButton(){}

        void CustomTextButton::drawButtonBackground (juce::Graphics& g,
                                                     juce::Button& button,
                                                   const juce::Colour& backgroundColour,
                                                   bool shouldDrawButtonAsHighlighted,
                                                   bool shouldDrawButtonAsDown)
        {
            auto cornerSize = 6.0f;
            auto bounds = button.getLocalBounds().toFloat().reduced (0.5f, 0.5f);

            auto baseColour = backgroundColour.withMultipliedSaturation (button.hasKeyboardFocus (true) ? 1.3f : 0.9f)
                                              .withMultipliedAlpha (button.isEnabled() ? 1.0f : 0.5f);

            if (shouldDrawButtonAsDown || shouldDrawButtonAsHighlighted)
                baseColour = baseColour.contrasting (shouldDrawButtonAsDown ? 0.2f : 0.05f);

            g.setColour (baseColour);
            g.fillRoundedRectangle(bounds, cornerSize);

            g.setColour (button.findColour (juce::ComboBox::outlineColourId));
            g.drawRoundedRectangle(bounds, cornerSize, 1.0f);
        }

    juce::Font CustomTextButton::getTextButtonFont (juce::TextButton&, int buttonHeight)
        {
            return juce::Font ("Helvetica", static_cast<float>(buttonHeight) * 0.5f, juce::Font::FontStyleFlags::bold);
        }






    


}
