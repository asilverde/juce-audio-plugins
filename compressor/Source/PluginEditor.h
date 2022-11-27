/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "GUI-Components.h"
#include "Meters.h"

class CompressorAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    CompressorAudioProcessorEditor(CompressorAudioProcessor&);
    ~CompressorAudioProcessorEditor() override;
    void paint(juce::Graphics&) override {}
    void resized() override;

private:
    CompressorAudioProcessor& audioProcessor;

    BgImage bgImage;
    PowerLine powerLine{ "Compressor", "Jacob Curtis", 30 };
    GainReductionMeter grMeter;
    SmallKnob thresholdKnob{ "Threshold", "dB"}, attackKnob{ "Attack", "ms" },
        releaseKnob{ "Release", "ms" }, ratioKnob{ "Ratio", ": 1" }, makeUpKnob{ "Make Up", "dB" },
        scFreqKnob{ "SC Freq", "Hz" }, mixKnob{ "Mix", "%" };
    SmallButton scBypassButton{ "SC Bypass" }, stereoButton{ "Stereo" };

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> thresholdAttach, attackAttach,
        releaseAttach, ratioAttach, makeUpAttach, scFreqAttach, mixAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> scBypassAttach, stereoAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CompressorAudioProcessorEditor)
};
