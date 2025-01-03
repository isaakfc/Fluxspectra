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
    void CustomDial2::drawRotarySlider
    (juce::Graphics &g,int x,int y,int width,int height,float sliderPos,float rotaryStartAngle,float rotaryEndAngle,juce::Slider &slider)
    {

        const auto outlineColor  = MyColours::logotext;
        const auto fillColor     = MyColours::blue;
        const auto mainColor     = MyColours::grey;
        const auto brighterColor = MyColours::background;
        const auto dialOutlineColor = MyColours::blackGrey;

        auto dialBounds = juce::Rectangle<int> (x, y, width, height).toFloat();
        auto centre = dialBounds.getCentre();
        auto fullRadius = juce::jmin (dialBounds.getWidth() / 2.0f, dialBounds.getHeight() / 2.0f);
        
        g.setColour (MyColours::logotext);

        for (int i = 0; i < 11; ++i)
        {
            
            auto dotSize = width * 0.025;

            const auto angle = juce::jmap (i / 10.0f, rotaryStartAngle, rotaryEndAngle);

            const auto point = centre.getPointOnCircumference (fullRadius - width * 0.06f, angle);

            g.fillEllipse (point.getX() - 3, point.getY() - 3, dotSize, dotSize);
        }

        fullRadius -= width / 14.5;

        auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        float lineWidthMultiplier = width * 0.035;
        auto lineWidth = juce::jmin (lineWidthMultiplier, fullRadius * 0.5f);

        auto arcRadius  = fullRadius - lineWidth * 2.25;

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

        g.setColour (outlineColor);
        g.strokePath (backgroundArc, juce::PathStrokeType (lineWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        auto dialRadius = std:: max (fullRadius - 4.0f * lineWidth, 10.0f);

        {
            juce::Graphics::ScopedSaveState saved (g);

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
        
            g.fillEllipse (centre.getX() - dialRadius, centre.getY() - dialRadius, dialRadius * 2.0f, dialRadius * 2.0f);
        }

        g.setColour (dialOutlineColor);
        auto scale = 2.0f;
        g.drawEllipse (centre.getX() - dialRadius, centre.getY() - dialRadius, dialRadius * scale, dialRadius * scale, 4.5f);

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

        g.setColour (fillColor);
        g.strokePath (dialValueTrack, juce::PathStrokeType (lineWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        g.setColour (dialOutlineColor);
        juce::Path dialTick;
        dialTick.startNewSubPath (centre.getPointOnCircumference (dialRadius - lineWidth, toAngle));
        dialTick.lineTo (centre.getPointOnCircumference ((dialRadius - lineWidth) * 0.6f, toAngle));

        g.strokePath (dialTick, juce::PathStrokeType (lineWidth * 0.75, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        _sliderWidth = slider.getWidth();
        
        g.setColour (MyColours::black);
        const juce::Font font (juce::Font ("Helvetica", 12, juce::Font::FontStyleFlags::bold));
        g.setFont (font);
        g.setFont(18.0f);
    }


void CustomDial2::drawLabel(juce::Graphics& g, juce::Label& label) 
    {
        g.fillAll(label.findColour(juce::Label::backgroundColourId));

        if (! label.isBeingEdited())
        {
            auto alpha = label.isEnabled() ? 1.0f : 0.5f;
            const juce::Font font(getLabelFont(label));
//            const juce::Font font (juce::Font ("Helvetica", 12, juce::Font::FontStyleFlags::bold));

            g.setColour(label.findColour(juce::Label::textColourId).withMultipliedAlpha(alpha));
            g.setFont(font);

            juce::Rectangle<int> textArea(label.getBorderSize().subtractedFrom(label.getLocalBounds()));
            
            const juce::Font font2 (juce::Font ("Helvetica", 12, juce::Font::FontStyleFlags::plain));
            g.setColour (MyColours::black);
            g.setFont (font2);
            g.setFont(12.0f);

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
            const juce::Font font (juce::Font ("Helvetica", _sliderWidth * 0.4, juce::Font::FontStyleFlags::bold));

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



// FOr band controls

CustomDial::CustomDial()
{
    
}
void CustomDial::drawRotarySlider
(
 juce::Graphics &g,
    int x,
    int y,
    int width,
    int height,
    float sliderPos,
    float rotaryStartAngle,
    float rotaryEndAngle,
 juce::Slider &slider
)
{
    _sliderName = slider.getName();
    
    const auto outlineColor  = slider.findColour (juce::Slider::rotarySliderOutlineColourId);
    const auto fillColor     = slider.findColour(juce::Slider::rotarySliderFillColourId);
    const auto mainColor     = slider.findColour(juce::Slider::thumbColourId).withAlpha(0.5f);
    const auto brighterColor = slider.findColour(juce::Slider::thumbColourId).withAlpha(0.5f).brighter(0.4f);
    const auto dialOutlineColor = slider.findColour (juce::Slider::backgroundColourId);

    auto dialBounds = juce::Rectangle<int> (x, y, width, height).toFloat();
    auto centre = dialBounds.getCentre();
    auto fullRadius = juce::jmin (dialBounds.getWidth() / 2.0f, dialBounds.getHeight() / 2.0f);
    
    g.setColour (juce::Colours::black);
    centre = dialBounds.getCentre();

    for (int i = 0; i < 11; ++i)
    {
        auto dotSize = width * 0.025;

        const auto angle = juce::jmap (i / 10.0f, rotaryStartAngle, rotaryEndAngle);

        const auto point = centre.getPointOnCircumference (fullRadius - width * 0.06f, angle);

        g.fillEllipse (point.getX() - 3, point.getY() - 3, dotSize, dotSize);
    }
        
    fullRadius -= width / 14.5;

    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    
    float lineWidthMultiplier = width * 0.035;
    auto lineWidth = juce::jmin (lineWidthMultiplier, fullRadius * 0.5f);
    auto arcRadius  = fullRadius - lineWidth * 2.25;

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

    g.setColour (outlineColor);
    g.strokePath (backgroundArc, juce::PathStrokeType (lineWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    auto dialRadius = std:: max (fullRadius - 4.0f * lineWidth, 10.0f);
    {
        juce::Graphics::ScopedSaveState saved (g);
        if (slider.isEnabled())
        {
            juce::ColourGradient fillGradient
            (
                brighterColor,
                centre.getX() + lineWidth * 0.9f,
                centre.getY() - lineWidth * 4.0f,
                mainColor.darker(1.0),
                centre.getX() + dialRadius,
                centre.getY() + dialRadius,
                true
            );
            
            g.setGradientFill (fillGradient);
        }
        
        g.fillEllipse (centre.getX() - dialRadius, centre.getY() - dialRadius, dialRadius * 2.0f, dialRadius * 2.0f);
    }
    
    g.setColour (juce::Colours::black);
    
    auto scale = 2.0f;
    
    g.drawEllipse (centre.getX() - dialRadius, centre.getY() - dialRadius, dialRadius * scale, dialRadius * scale, 4.5f);
            
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

    g.setColour (fillColor);
    g.strokePath (dialValueTrack, juce::PathStrokeType (lineWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    
    g.setColour (juce::Colours::black);
    juce::Path dialTick;
    dialTick.startNewSubPath (centre.getPointOnCircumference (dialRadius - lineWidth, toAngle));
    
    dialTick.lineTo (centre.getPointOnCircumference ((dialRadius - lineWidth) * 0.6f, toAngle));
    
    g.strokePath (dialTick, juce::PathStrokeType (lineWidth * 0.75, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    
    _sliderWidth = slider.getWidth();
}

void CustomDial::drawLabel (juce::Graphics& g, juce::Label& label)
{
    g.fillAll (label.findColour (Label::backgroundColourId));

    label.setEditable(true);
    
    if (! label.isBeingEdited())
    {
        auto alpha = label.isEnabled() ? 1.0f : 0.5f;
        const juce::Font font (juce::Font ("Helvetica", 12, juce::Font::FontStyleFlags::bold));

        g.setColour (label.findColour (Label::textColourId).withMultipliedAlpha (alpha));
        g.setFont (font);

        auto textArea = getLabelBorderSize (label).subtractedFrom (label.getLocalBounds());
        
        juce::String labelText;
        if (auto* parentComponent = label.getParentComponent())
        {
            if (auto* slider = dynamic_cast<juce::Slider*>(parentComponent))
            {
                // Check if the mouse is over the slider
                bool isMouseOver = slider->isMouseOver() || slider->isMouseButtonDown();
                
                // Get the slider value and suffix
                float value;
                
                if (_dialValueType == ValueType::kInt)
                {
                    value = static_cast<int>(slider->getValue());
                }
                
                else
                {
                    value = slider->getValue();
                }
                
                juce::String suffix = slider->getTextValueSuffix();
                
                // Determine the text to display based on the mouse over state
                if (isMouseOver)
                {
                    labelText = juce::String(value) + suffix;
                }
                else
                {
                    labelText = slider->getName();
                }
            }
        }
        
        g.drawFittedText (labelText, textArea, label.getJustificationType(),
                          juce::jmax (1, (int) ((float) textArea.getHeight() / font.getHeight())),
                          label.getMinimumHorizontalScale());

        g.setColour (label.findColour (Label::outlineColourId).withMultipliedAlpha (alpha));
    }
    
    else if (label.isEnabled())
    {
        g.setColour (label.findColour (Label::outlineColourId));
    }

    g.drawRect (label.getLocalBounds());
}


    
DialBc::DialBc(const juce::String& sliderName, const juce::String& suffix)
{
    dial.setName(sliderName);
    
    // Slider props
    dial.setRange(-15.0, 15.0, 0.1);
    dial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 32);
    dial.setTextValueSuffix(suffix);
    dial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    dial.setDoubleClickReturnValue(true, 0.0);
    dial.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::transparentBlack);
    dial.setColour(juce::Slider::ColourIds::backgroundColourId, _auxBackgroundColor.withAlpha(0.35f));
    dial.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, _auxBackgroundColor.withAlpha(0.35f));
    dial.setColour(juce::Slider::ColourIds::textBoxTextColourId, _mainTextColor);
    dial.setColour(juce::Slider::ColourIds::trackColourId, _widgetFillColor.withAlpha(0.75f));
    dial.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, _widgetFillColor.withAlpha(0.75f));
    dial.setColour(juce::Slider::ColourIds::thumbColourId, _auxTextColor);
    dial.setLookAndFeel(&_customDial);
    addAndMakeVisible(dial);
}

DialBc::~DialBc()
{
    setLookAndFeel(nullptr);
}

void DialBc::paint (juce::Graphics& g)
{
}

void DialBc::resized()
{
    dial.setBounds(getLocalBounds());
}

void DialBc::setDialTextBoxWidth(const float newWidth)
{
    dial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, newWidth, 32);
}

void DialBc::setDialColors(juce::Colour mainText,
                                     juce::Colour widgetFill,
                                     juce::Colour auxBG,
                                     juce::Colour auxText)
{
    dial.setColour(juce::Slider::ColourIds::backgroundColourId, auxBG);
    dial.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, auxBG);
    dial.setColour(juce::Slider::ColourIds::textBoxTextColourId, mainText);
    dial.setColour(juce::Slider::ColourIds::trackColourId, widgetFill);
    dial.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, widgetFill);
    dial.setColour(juce::Slider::ColourIds::thumbColourId, auxText);
}

void DialBc::setDialValueType(CustomDial::ValueType newValueType)
{
    _customDial.setDialValueType(newValueType);
    repaint();
}


void DialBc::setDialSide()
{
    dial.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 32);
}


}

