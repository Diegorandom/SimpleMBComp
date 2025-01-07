/*
  ==============================================================================

    lookAndFeel.h
    Created: 6 Jan 2025 8:20:39pm
    Author:  Diego Ortega

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

struct LookAndFeel : juce::LookAndFeel_V4
{
    void drawRotarySlider(juce::Graphics & g,
                          int     x,
                          int     y,
                          int     width,
                          int     height,
                          float     sliderPosProportional,
                          float     rotaryStartAngle,
                          float     rotaryEndAngle,
                          juce::Slider & slider
                          ) override;
    
    void drawToggleButton (juce::Graphics & g,
                               juce::ToggleButton & button,
                               bool shouldDrawButtonAsHighlighted,
                               bool shouldDrawButtonAsDown) override;
};
