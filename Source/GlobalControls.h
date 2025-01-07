/*
  ==============================================================================

    GlobalControls.h
    Created: 7 Jan 2025 1:21:59pm
    Author:  Diego Ortega

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "RotarySliderWithLabels.h"
#include "Utilities.h"

struct GlobalControls : juce::Component
{
    GlobalControls(juce::AudioProcessorValueTreeState& apvts);
    
    void paint(juce::Graphics& g) override;
    
    void resized() override;
    
private:
    using RSWL = RotarySliderWithLabels;
    std::unique_ptr<RSWL> inGainSlider, lowMidXoverSlider, midHighXoverSlider, outGainSlider;
    
    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<Attachment> lowMidXoverSliderAttachment,
                               midHighXoverSliderAttachment,
                               inGainSliderAttachment,
                               outGainSliderAttachment;
};
