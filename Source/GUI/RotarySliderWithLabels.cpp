/*
  ==============================================================================

    RotarySliderWithLabels.cpp
    Created: 7 Jan 2025 12:44:09pm
    Author:  Diego Ortega

  ==============================================================================
*/

#include "RotarySliderWithLabels.h"
#include "Utilities.h"

juce::Rectangle<int> RotarySliderWithLabels::getSliderBounds() const
{
    auto bounds = getLocalBounds();
    bounds.removeFromTop(getTextHeight() * 1.5);
    auto size = juce::jmin(bounds.getWidth(), bounds.getHeight());
    size -= getTextHeight() * 1.5;
    juce::Rectangle<int> r;
    r.setSize(size, size);
    r.setCentre(bounds.getCentreX(), 0);
//    r.setY(2);
    r.setY(bounds.getY());
    return r;
}

void RotarySliderWithLabels::paint(juce::Graphics &g)
{
    using namespace juce;
    auto startAng = degreesToRadians(180.f + 45.f);
    auto endAng = degreesToRadians(180.f - 45.f) + MathConstants<float>::twoPi;
    auto range = getRange();
    auto sliderBounds = getSliderBounds();
    auto bounds = getLocalBounds();
    g.setColour(Colours::blueviolet);
    g.drawFittedText(getName(), bounds.removeFromTop(getTextHeight() + 2), Justification::centredBottom, 1);

//    g.setColour(Colours::red);
//    g.drawRect(getLocalBounds());
//    g.setColour(Colours::yellow);
//    g.drawRect(sliderBounds);
       
    getLookAndFeel().drawRotarySlider(g,
                                      sliderBounds.getX(),
                                      sliderBounds.getY(),
                                      sliderBounds.getWidth(),
                                      sliderBounds.getHeight(),
                                      /*
                                       don't forget that we need the normalized value here.
                                       */
                                      jmap(getValue(), range.getStart(), range.getEnd(), 0.0, 1.0),
                                      startAng,
                                      endAng,
                                      *this);
    
    auto center = sliderBounds.toFloat().getCentre();
    auto radius = sliderBounds.getWidth() * 0.5f;
    
    g.setColour(Colour(0u, 172u, 1u));
    g.setFont(getTextHeight());
    
    auto numChoices = labels.size();
        for( int i = 0; i < numChoices; ++i )
        {
            auto pos = labels[i].pos;
                    jassert( 0.f <= pos );
                    jassert( pos <= 1.f );
                    auto ang = jmap(labels[i].pos, 0.f, 1.f, startAng, endAng);
            
            auto c = center.getPointOnCircumference(radius + getTextHeight() * 0.5f + 1, ang);
            
            Rectangle<float> r;
            auto str = labels[i].label;
            
            r.setSize(g.getCurrentFont().getStringWidth(str), getTextHeight());
            
            r.setCentre(c);
            
            r.setY(r.getY() + getTextHeight());
            
            g.drawFittedText(str, r.toNearestInt(), Justification::centred, 1);
        }
}

// Let's keep this simple and just return the string that has the perimeter value.
juce::String RotarySliderWithLabels::getDisplayString() const
{
    // return juce::String(getValue());
    
    if (auto* choiceParam = dynamic_cast<juce::AudioParameterChoice*>(param))
        return choiceParam->getCurrentChoiceName();
    
    juce::String str;
        
    bool addK = false;
    
    if( auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(param) )
        {
            float val = getValue();
//            if( val > 999.f)
//                    {
//                        val /= 1000.f;
//                        addK = true;
//                    }
            addK = truncateKiloValue(val);
            str = juce::String(val,(addK ? 2 : 0));
        }
        else
        {
            jassertfalse; //uh oh, no float param!!
        }
    
    if( suffix.isNotEmpty() )
        {
            str << " ";
            if( addK )
                str << "k";
            str << suffix;
        }
    
    return str;
}

void RotarySliderWithLabels::changeParam(juce::RangedAudioParameter *p)
{
    param = p;
    repaint();
}

juce::String RatioSlider::getDisplayString() const
{
    auto choiceParam = dynamic_cast<juce::AudioParameterChoice*>(param);
    jassert(choiceParam != nullptr);
    
    auto currentChoice = choiceParam->getCurrentChoiceName();
    if( currentChoice.contains(".0"))
       currentChoice = currentChoice.substring(0, currentChoice.indexOf("."));
    
    currentChoice << ":1";
    
    return currentChoice;
}
