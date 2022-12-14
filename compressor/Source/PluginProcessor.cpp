/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

CompressorAudioProcessor::CompressorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ),
    parameters(*this, nullptr)
#endif
{
    parameters.createAndAddParameter(std::make_unique<juce::AudioParameterFloat>(ParameterID{"threshold", 1},
        "Threshold", juce::NormalisableRange<float>(-50.0f, 0.0f, 0.1f), 0.0f, "dB"));
    parameters.createAndAddParameter(std::make_unique<juce::AudioParameterFloat>(ParameterID{"attack", 2},
        "Attack Time", juce::NormalisableRange<float>(0.5f, 100.0f, 0.5f), 10.0f, "ms"));
    parameters.createAndAddParameter(std::make_unique<juce::AudioParameterFloat>(ParameterID{"release", 3},
        "Release Time", juce::NormalisableRange<float>(1.0f, 1100.0f, 1.0f), 50.0f, "ms"));
    parameters.createAndAddParameter(std::make_unique<juce::AudioParameterFloat>(ParameterID{"ratio",4},
        "Ratio", juce::NormalisableRange<float>(1.0f, 16.0f, 1.0f), 4.0f, " : 1"));
    parameters.createAndAddParameter(std::make_unique<juce::AudioParameterFloat>(ParameterID{"makeUp", 5},
        "MakeUp Gain", juce::NormalisableRange<float>(-10.0f, 20.0f, 0.1f), 0.0f, "dB"));
    parameters.createAndAddParameter(std::make_unique<juce::AudioParameterFloat>(ParameterID{"scFreq", 6},
        "Side Chain Frequency", juce::NormalisableRange<float>(20.0f, 2000.0f, 1.0f), 20.0f, "Hz"));
    parameters.createAndAddParameter(std::make_unique<juce::AudioParameterBool>(ParameterID{"scBypass", 7},
        "Side Chain Bypass", true));
    parameters.createAndAddParameter(std::make_unique<juce::AudioParameterBool>(ParameterID{"stereo", 8},
        "Stereo Mode", true));
    parameters.createAndAddParameter(std::make_unique<juce::AudioParameterFloat>(ParameterID{"mix", 9},
        "Mix", juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f), 100.0f, "%"));
    parameters.state = juce::ValueTree("savedParams");
}

void CompressorAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    compressor.prepare(sampleRate, samplesPerBlock);
    compressor.setParameters(parameters);
}

void CompressorAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    for (auto i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    compressor.setParameters(parameters);
    compressor.process(buffer);
    gainReduction = compressor.getGainReduction();
}

void CompressorAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // create xml with state information
    std::unique_ptr <juce::XmlElement> outputXml(parameters.state.createXml());
    // save xml to binary
    copyXmlToBinary(*outputXml, destData);
}

void CompressorAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // create xml from binary
    std::unique_ptr<juce::XmlElement> inputXml(getXmlFromBinary(data, sizeInBytes));
    // check that theParams returned correctly
    if (inputXml != nullptr)
    {
        // if theParams tag name matches tree state tag name
        if (inputXml->hasTagName(parameters.state.getType()))
        {
            // copy xml into tree state
            parameters.state = juce::ValueTree::fromXml(*inputXml);
        }
    }
}

//==============================================================================
//==============================================================================
//==============================================================================

bool CompressorAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool CompressorAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool CompressorAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CompressorAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif
    return true;
#endif
}
#endif

juce::AudioProcessorEditor* CompressorAudioProcessor::createEditor()
{
    return new CompressorAudioProcessorEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CompressorAudioProcessor();
}
