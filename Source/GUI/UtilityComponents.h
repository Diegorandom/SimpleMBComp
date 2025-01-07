/*
  ==============================================================================

    UtilityComponents.h
    Created: 7 Jan 2025 12:55:51pm
    Author:  Diego Ortega

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

struct Placeholder : juce::Component
{
  Placeholder();
    
    void paint(juce::Graphics& g) override;
    
    juce::Colour customColor;
};

struct RotarySlider : juce::Slider
{
    RotarySlider();
};
