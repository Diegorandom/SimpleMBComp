/*
  ==============================================================================

    PathProducer.h
    Created: 8 Jan 2025 2:23:45pm
    Author:  Diego Ortega

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "FFTDataGenerator.h"
#include "AnalyzerPathGenerator.h"
#include "../PluginProcessor.h"


struct PathProducer
{
    PathProducer(SingleChannelSampleFifo<SimpleMBCompAudioProcessor::BlockType>& scsf) :
    leftChannelFifo(&scsf)
    {
        leftChannelFFTDataGenerator.changeOrder(FFTOrder::order2048);
        monoBuffer.setSize(1, leftChannelFFTDataGenerator.getFFTSize());
    }
    void process(juce::Rectangle<float> fftBounds, double sampleRate);
    
    juce::Path getPath() { return leftChannelFFTPath; }

private:
    SingleChannelSampleFifo<SimpleMBCompAudioProcessor::BlockType>* leftChannelFifo;
    juce::AudioBuffer<float> monoBuffer;
    FFTDataGenerator<std::vector<float>> leftChannelFFTDataGenerator;
    AnalyzerPathGenerator<juce::Path> pathProducer;
    juce::Path leftChannelFFTPath;
};
