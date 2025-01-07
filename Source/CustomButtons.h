/*
  ==============================================================================

    CustomButtons.h
    Created: 7 Jan 2025 12:47:29pm
    Author:  Diego Ortega

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

struct PowerButton : juce::ToggleButton { };
struct AnalyzerButton : juce::ToggleButton {
    void resized() override;
    
    juce::Path randomPath;
};
