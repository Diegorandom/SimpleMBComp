/*
  ==============================================================================

    UtilityComponents.cpp
    Created: 7 Jan 2025 12:55:51pm
    Author:  Diego Ortega

  ==============================================================================
*/

#include "UtilityComponents.h"

Placeholder::Placeholder()
{
    juce::Random r;
    customColor = juce::Colour(r.nextInt(255), r.nextInt(255), r.nextInt(255));
}

void Placeholder::paint(juce::Graphics& g)
  {
      g.fillAll(customColor);
  }


RotarySlider::RotarySlider() :
  juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
               juce::Slider::TextEntryBoxPosition::NoTextBox)
  {}
