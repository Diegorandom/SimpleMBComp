/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

template<typename T>
bool truncateKiloValue(T& value)
{
    if(value > static_cast<T>(999))
    {
        value /= static_cast<T>(1000);
        return true;
    }
}

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
    juce::Path randomPath;
}

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
    
     g.setColour(Colour(97u, 18u, 167u)); //97 18 167
     g.fillEllipse(bounds);
     g.setColour(Colour(255u, 154u, 1u) );
     g.drawEllipse(bounds, 1.f);
    
    if( auto* rswl = dynamic_cast<RotarySliderWithLabels*>(&slider) )
    {
        
        
        auto enabled = slider.isEnabled();
        
        g.setColour(enabled ? Colour(97u, 18u, 167u) : Colours::darkgrey );
            g.fillEllipse(bounds);
            
            g.setColour(enabled ? Colour(255u, 154u, 1u) : Colours::grey);
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
}

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

Placeholder::Placeholder()
{
    juce::Random r;
    customColor = juce::Colour(r.nextInt(255), r.nextInt(255), r.nextInt(255));
}

CompressorBandControls::CompressorBandControls(juce::AudioProcessorValueTreeState& apv) :
apvts(apv),
attackSlider(nullptr, "ms", "ATTACK"),
releaseSlider(nullptr, "ms", "RELEASE"),
thresholdSlider(nullptr, "dB", "THRESH"),
ratioSlider(nullptr, "")
{
    using namespace Params;
    const auto& params = GetParams();
    
    auto getParamHelper = [&params, &apvts = this->apvts](const auto& name) -> auto&
    {
        return getParam(apvts, params, name);
    };
    
    attackSlider.changeParam(&getParamHelper(Names::Attack_Mid_Band));
    releaseSlider.changeParam(&getParamHelper(Names::Release_Mid_Band));
    thresholdSlider.changeParam(&getParamHelper(Names::Threshold_Mid_Band));
    ratioSlider.changeParam(&getParamHelper(Names::Ratio_Mid_Band));
    
    addLabelPairs(attackSlider.labels, getParamHelper(Names::Attack_Mid_Band), "ms");
    addLabelPairs(releaseSlider.labels, getParamHelper(Names::Release_Mid_Band), "ms");
    addLabelPairs(thresholdSlider.labels, getParamHelper(Names::Threshold_Mid_Band), "dB");

    ratioSlider.labels.add({0.f, "1:1"});
    auto ratioParam = dynamic_cast<juce::AudioParameterChoice*>(&getParamHelper(Names::Ratio_Mid_Band));
    ratioSlider.labels.add({1.0f,
        juce::String(ratioParam->choices.getReference(ratioParam->choices.size() -1 ).getIntValue()) + ":1" });
    
    auto makeAttachmentHelper = [&params, &apvts = this->apvts](auto& attachment, const auto& name, auto& slider)
    {
        makeAttachment(attachment, apvts, params, name, slider);
    };
    
    makeAttachmentHelper(attackSliderAttachment, Names::Attack_Mid_Band, attackSlider);
    makeAttachmentHelper(releaseSliderAttachment, Names::Release_Mid_Band, releaseSlider);
    makeAttachmentHelper(thresholdSliderAttachment, Names::Threshold_Mid_Band, thresholdSlider);
    makeAttachmentHelper(ratioSliderAttachment, Names::Ratio_Mid_Band, ratioSlider);
    
    addAndMakeVisible(attackSlider);
    addAndMakeVisible(releaseSlider);
    addAndMakeVisible(thresholdSlider);
    addAndMakeVisible(ratioSlider);
}

void CompressorBandControls::resized()
{
    auto bounds = getLocalBounds().reduced(5);
    using namespace juce;
    
    FlexBox flexBox;
    flexBox.flexDirection = FlexBox::Direction::row;
    flexBox.flexWrap = FlexBox::Wrap::noWrap;
    
    auto spacer = FlexItem().withWidth(4);
    auto endCap = FlexItem().withWidth(6);
    
    flexBox.items.add(endCap);
    flexBox.items.add(FlexItem(attackSlider).withFlex(1.f));
    flexBox.items.add(spacer);
    flexBox.items.add(FlexItem(releaseSlider).withFlex(1.f));
    flexBox.items.add(spacer);
    flexBox.items.add(FlexItem(thresholdSlider).withFlex(1.f));
    flexBox.items.add(spacer);
    flexBox.items.add(FlexItem(ratioSlider).withFlex(1.f));
    flexBox.items.add(endCap);
    
    flexBox.performLayout(bounds);
}

void drawmoduleBackground(juce::Graphics &g, juce::Rectangle<int> bounds)
{
    using namespace juce;
    g.setColour(Colours::blueviolet);
    g.fillAll();
    
    auto localBounds = bounds;
    
    bounds.reduce(3, 3);
    g.setColour(Colours::black);
    g.fillRoundedRectangle(bounds.toFloat(), 3);
    
    g.drawRect(localBounds);
}

void CompressorBandControls::paint(juce::Graphics &g)
{
//    using namespace juce;
    auto bounds = getLocalBounds();
//    g.setColour(Colours::blueviolet);
//    g.fillAll();
//
//    auto localBounds = bounds;
//
//    bounds.reduce(3, 3);
//    g.setColour(Colours::black);
//    g.fillRoundedRectangle(bounds.toFloat(), 3);
//
//    g.drawRect(localBounds);
    
    drawmoduleBackground(g, bounds);
}

GlobalControls::GlobalControls(juce::AudioProcessorValueTreeState& apvts)
{
    using namespace Params;
    const auto& params = GetParams();
    
    auto getParamHelper = [&params, &apvts](const auto& name) -> auto&
    {
        return getParam(apvts, params, name);
    };
    
    auto& gainInParam = getParamHelper(Names::Gain_In);
    auto& lowMidParam = getParamHelper(Names::Low_Mid_Crossover_Freq);
    auto& midHighParam = getParamHelper(Names::Mid_High_Crossover_Freq);
    auto& gainOutParam = getParamHelper(Names::Gain_Out);
    
    inGainSlider = std::make_unique<RSWL>(&gainInParam, "dB", "INPUT TRIM");
    lowMidXoverSlider = std::make_unique<RSWL>(&lowMidParam, "Hz", "LOW-MID X-OVER");
    midHighXoverSlider = std::make_unique<RSWL>(&midHighParam, "Hz", "MID-HI X-OVER");
    outGainSlider = std::make_unique<RSWL>(&gainOutParam, "dB", "OUTPUT TRIM");

    auto makeAttachmentHelper = [&params, &apvts](auto& attachment, const auto& name, auto& slider)
    {
        makeAttachment(attachment, apvts, params, name, slider);
    };
    
    makeAttachmentHelper(inGainSliderAttachment, Names::Gain_In, *inGainSlider);
    makeAttachmentHelper(lowMidXoverSliderAttachment, Names::Low_Mid_Crossover_Freq, *lowMidXoverSlider);
    makeAttachmentHelper(midHighXoverSliderAttachment, Names::Mid_High_Crossover_Freq, *midHighXoverSlider);
    makeAttachmentHelper(outGainSliderAttachment, Names::Gain_Out, *outGainSlider);
    
    addLabelPairs(inGainSlider->labels, gainInParam, "dB");
    addLabelPairs(lowMidXoverSlider->labels, lowMidParam, "Hz");
    addLabelPairs(midHighXoverSlider->labels, midHighParam, "Hz");
    addLabelPairs(outGainSlider->labels, gainOutParam, "dB");

    
    addAndMakeVisible(*inGainSlider);
    addAndMakeVisible(*lowMidXoverSlider);
    addAndMakeVisible(*midHighXoverSlider);
    addAndMakeVisible(*outGainSlider);
}

void GlobalControls::paint(juce::Graphics &g)
{
//    using namespace juce;
    auto bounds = getLocalBounds();
//    g.setColour(Colours::blueviolet);
//    g.fillAll();
//
//    auto localBounds = bounds;
//
//    bounds.reduce(3, 3);
//    g.setColour(Colours::black);
//    g.fillRoundedRectangle(bounds.toFloat(), 3);
    
//    g.drawRect(localBounds);
    
    drawmoduleBackground(g, bounds);
}

void GlobalControls::resized()
{
    auto bounds = getLocalBounds().reduced(5);
    using namespace juce;
    
    FlexBox flexBox;
    flexBox.flexDirection = FlexBox::Direction::row;
    flexBox.flexWrap = FlexBox::Wrap::noWrap;
    
    auto spacer = FlexItem().withWidth(4);
    auto endCap = FlexItem().withWidth(6);
    
    flexBox.items.add(endCap);
    flexBox.items.add(FlexItem(*inGainSlider).withFlex(1.f));
    flexBox.items.add(spacer);
    flexBox.items.add(FlexItem(*lowMidXoverSlider).withFlex(1.f));
    flexBox.items.add(spacer);
    flexBox.items.add(FlexItem(*midHighXoverSlider).withFlex(1.f));
    flexBox.items.add(spacer);
    flexBox.items.add(FlexItem(*outGainSlider).withFlex(1.f));
    flexBox.items.add(endCap);
    
    flexBox.performLayout(bounds);
}
//==============================================================================
SimpleMBCompAudioProcessorEditor::SimpleMBCompAudioProcessorEditor (SimpleMBCompAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setLookAndFeel(&lnf);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
//    addAndMakeVisible(controlBar);
//    addAndMakeVisible(analyzer);
    addAndMakeVisible(globalControls);
    addAndMakeVisible(bandControls);
    setSize (600, 500);
}

SimpleMBCompAudioProcessorEditor::~SimpleMBCompAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void SimpleMBCompAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
//    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
//
//    g.setColour (juce::Colours::white);
//    g.setFont (15.0f);
//    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
    g.fillAll(juce::Colours::black);
}

void SimpleMBCompAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    auto bounds = getLocalBounds();
    controlBar.setBounds(bounds.removeFromTop(32));
    bandControls.setBounds(bounds.removeFromBottom(135));
    analyzer.setBounds(bounds.removeFromTop(225));
    globalControls.setBounds(bounds);
}
