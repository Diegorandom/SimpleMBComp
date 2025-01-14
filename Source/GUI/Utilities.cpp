/*
  ==============================================================================

    Utilities.cpp
    Created: 7 Jan 2025 12:59:02pm
    Author:  Diego Ortega

  ==============================================================================
*/

#include "Utilities.h"
#include "LookAndFeel.h"

juce::String getValString(const juce::RangedAudioParameter& param,
                          bool getLow,
                          juce::String suffix)
{
    juce::String str;
    
    auto val = getLow ? param.getNormalisableRange().start : param.getNormalisableRange().end;
    
    bool useK = truncateKiloValue(val);
    str << val;
    if(useK)
        str << "k";
    
    str << suffix;
    
    return str;
}


template<typename T>
bool truncateKiloValue(T& value)
{
    if(value > static_cast<T>(999))
    {
        value /= static_cast<T>(1000);
        return true;
    }
}

juce::Rectangle<int> drawmoduleBackground(juce::Graphics &g, juce::Rectangle<int> bounds)
{
    using namespace juce;
    g.setColour(ColorScheme::getModuleBorderColor());
//    g.setColour(Colours::blueviolet);
    g.fillAll();
    
    auto localBounds = bounds;
    
    bounds.reduce(3, 3);
    g.setColour(Colours::black);
    g.fillRoundedRectangle(bounds.toFloat(), 3);
    
    g.drawRect(localBounds);
    
    return bounds;
}
