/*
  ==============================================================================

    lookAndFeel.cpp
    Created: 6 Jan 2025 8:20:39pm
    Author:  Diego Ortega

  ==============================================================================
*/

#include "lookAndFeel.h"
#include "RotarySliderWithLabels.h"
#include "CustomButtons.h"

void LookAndFeel::drawRotarySlider(juce::Graphics &g,
                                   int x,
                                   int y,
                                   int width,
                                   int height,
                                   float sliderPosProportional,
                                   float rotaryStartAngle,
                                   float rotaryEndAngle,
                                   juce::Slider &slider)
{
    using namespace juce;
    auto bounds = Rectangle<float>(x,y,width,height);
    auto enabled = slider.isEnabled();
    
//     g.setColour(enabled ? Colour(97u, 18u, 167u) : Colours::darkgrey); //97 18 167
    g.setColour(enabled ? ColorScheme::getModuleBorderColor() : Colours::green);

    g.fillEllipse(bounds);
//     g.setColour(Colour(255u, 154u, 1u) );

    g.setColour(enabled ? ColorScheme::getSliderBorderColor() : Colours::grey);
    g.drawEllipse(bounds, 1.f);
    
    if( auto* rswl = dynamic_cast<RotarySliderWithLabels*>(&slider))
    {
        auto center = bounds.getCentre();
        Path p;
        
        Rectangle<float> r;
        r.setLeft( center.getX() - 2 );
        r.setRight( center.getX() + 2 );
        r.setTop( bounds.getY());
        r.setBottom(center.getY() - rswl->getTextHeight()*1.5);
        
        p.addRoundedRectangle(r, 2.f); //this rectangle is at 12 o'clock (0 radians).
        
        auto sliderAngRad = jmap(sliderPosProportional,
                                0.f, 1.f,
                                rotaryStartAngle,
                                rotaryEndAngle);

        p.applyTransform(AffineTransform().rotated(sliderAngRad,
                                                  center.getX(),
                                                  center.getY()));

        g.fillPath(p);
        
        // Now let's make a rectangle that is a little bit wider than our
        // text and a little bit taller than our text is.
        g.setFont(rswl->getTextHeight());
        auto text = rswl->getDisplayString();
        auto strWidth = g.getCurrentFont().getStringWidth(text);
        r.setSize(strWidth + 4, rswl->getTextHeight() + 2);
        
        // And we will set the center of it to be the center of our circle.
        r.setCentre(bounds.getCentre());
        
        
        // Now make the background black and fill it in.
        g.setColour(enabled ? Colours::black : Colours::darkgrey);
        g.fillRect(r);
        
        
        // paint our text as white text within our rectangle.
        g.setColour(enabled ? Colours::white : Colours::lightgrey);
        g.drawFittedText(text, r.toNearestInt(), juce::Justification::centred, 1);
    }
}


void LookAndFeel::drawToggleButton(juce::Graphics &g, juce::ToggleButton &button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    using namespace juce;
    
    if( auto* pb = dynamic_cast<PowerButton*>(&button) )
    {
        juce::Path powerButton;
        
        auto bounds = button.getLocalBounds();
        auto size = juce::jmin(bounds.getWidth(), bounds.getHeight()) - 6; //JUCE_LIVE_CONSTANT(5);
        auto r = bounds.withSizeKeepingCentre(size, size).toFloat();
        
        float ang = 30.f; //JUCE_LIVE_CONSTANT(30);
        
        size -= 6; //JUCE_LIVE_CONSTANT(5);
        
        powerButton.addCentredArc(r.getCentreX(),   //x
                                  r.getCentreY(),   //y
                                  size * 0.5,       //radius x
                                  size * 0.5,       //radius y
                                  0.f,              //circle rotation
                                  juce::degreesToRadians(ang), //start ang
                                  juce::degreesToRadians(360.f - ang), //end ang
                                  true );  //start as subPath
        
        powerButton.startNewSubPath(r.getCentreX(), r.getY());
        powerButton.lineTo(r.getCentre());
        
        juce::PathStrokeType pst(2, //JUCE_LIVE_CONSTANT(3),
                                 juce::PathStrokeType::JointStyle::curved);
        
        auto color = button.getToggleState() ? Colours::dimgrey : Colour(0u, 172u, 1u);
        
        g.setColour(color);
        g.strokePath(powerButton, pst);
        g.drawEllipse(r, 2);
    } else if( auto* analyzerButton = dynamic_cast<AnalyzerButton*>(&button) )
    {
        auto color = ! button.getToggleState() ? Colours::dimgrey : Colour(0u, 172u, 1u);
        g.setColour(color);
        auto bounds = button.getLocalBounds();
        g.drawRect(bounds);
        g.strokePath(analyzerButton ->randomPath, PathStrokeType(1.f));
    }
    else
    {
        auto bounds = button.getLocalBounds().reduced(2);
        auto buttonIsOn = button.getToggleState();
        const int cornerSize = 4;
        g.setColour(buttonIsOn ? button.findColour(TextButton::ColourIds::buttonOnColourId) :
                    button.findColour(TextButton::ColourIds::buttonColourId));
        g.fillRoundedRectangle(bounds.toFloat(), cornerSize);
        g.setColour(buttonIsOn ? juce::Colours::black : juce::Colours::white);
        g.drawRoundedRectangle(bounds.toFloat(), cornerSize,1);
        g.drawFittedText(button.getName(), bounds, juce::Justification::centred, 1);
    }
}

