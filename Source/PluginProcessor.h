/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

/**
 DSP Roadmap
 1) Figure out how to split the audio into 3 bands - DONE
 2) Create parameters to control where this split happens - DONE
 3) Prove that splitting into 3 bands produces no audible artifacts - DONE
 4) Create audio parameters for the 3 compressor bands. These need to live on each band instance. - DONE
 5) Add 2 remaining compressors - DONE
 6) Add ability to mute/solo/bypass individual compressors - DONE
 7) Add input and output gain to offset changes in output level - DONE
 8) Clean up anything that needs cleaning up - DONE


 GUI Roadmap
 1) Global Control (x-over sliders, gain sliders) - DONE
 2) Main Band Controls (attack, release, threshold, ratio) - DONE
 3) Add Solo/Mute/Bypass Buttons - DONE
 4) Band Select Functionality - DONE
 5) Band Select Buttons reflect the Solo/Mute/Bypass state - DONE
 6) Custom Look and Feel for Sliders and Toggle Buttons - DONE
 7) Spectrum Analyzer Overview
 8) Data Structures for Spectrum Analyzer
 9) Fifo usage in pluginProcessor::processBlock
 10) Implementation of the Analyzer Rendering Pre-computed Paths.
 11) Drawing Cross-overs on top of the Analyzer Plot.
 12) Drawing Gain Reduction on top of the Analyzer.
 13) Analyzer Bypass
 14) Global Bypass Button
 */


#include <JuceHeader.h>
#include "DSP/CompressorBand.h"
#include "DSP/SingleChannelSampleFifo.h"

/**
*/
class SimpleMBCompAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    SimpleMBCompAudioProcessor();
    ~SimpleMBCompAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    using APVTS = juce::AudioProcessorValueTreeState;
    static APVTS::ParameterLayout createParameterLayout();
    
    APVTS apvts {*this, nullptr, "Parameters", createParameterLayout()};
    
    using BlockType = juce::AudioBuffer<float>;
    SingleChannelSampleFifo<BlockType> leftChannelFifo { Channel::Left };
    SingleChannelSampleFifo<BlockType> rightChannelFifo { Channel::Right };

private:    
    std::array<CompressorBand, 3> compressors;
    CompressorBand& lowBandComp = compressors[0];
    CompressorBand& midBandComp = compressors[1];
    CompressorBand& highBandComp = compressors[2];
    
    using Filter = juce::dsp::LinkwitzRileyFilter<float>;
    //     Fc0  Fc1
    Filter LP1, AP2,
           HP1, LP2,
                HP2;
    
//    Filter invAP1, invAP2;
//    juce::AudioBuffer<float> invAPBuffer;
    
    juce::AudioParameterFloat* lowMidCrossover { nullptr };
    juce::AudioParameterFloat* midHighCrossover { nullptr };
    
    std::array<juce::AudioBuffer<float>, 3> filterBuffers;
    
    juce::dsp::Gain<float> inputGain, outputGain;
    juce::AudioParameterFloat* inputGainParam { nullptr };
    juce::AudioParameterFloat* outputGainParam { nullptr };
    
    template<typename T, typename U>
    void applyGain(T& buffer, U& gain)
    {
        auto block = juce::dsp::AudioBlock<float>(buffer);
        auto ctx = juce::dsp::ProcessContextReplacing<float>(block);
        gain.process(ctx);
    }
    
    void updateState();
    
    void splitBands(const juce::AudioBuffer<float>& inputBuffer);
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleMBCompAudioProcessor)
};
