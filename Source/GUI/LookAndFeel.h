/*
  ==============================================================================

    lookAndFeel.h
    Created: 6 Jan 2025 8:20:39pm
    Author:  Diego Ortega

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#define USE_LIVE_CONSTANT true

#if USE_LIVE_CONSTANT
#define colorHelper(c) JUCE_LIVE_CONSTANT(c);
#else
#define colorHelper(c) c;
#endif

namespace ColorScheme
{
inline juce::Colour getSliderBorderColor()
{
    return colorHelper( juce::Colour(0xff27869c) );
}

inline juce::Colour getModuleBorderColor()
{
    return colorHelper( juce::Colours::blueviolet );
}
}


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
