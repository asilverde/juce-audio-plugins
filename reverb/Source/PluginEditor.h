/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/


#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "GUI-Components.h"

class ReverbAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    ReverbAudioProcessorEditor(ReverbAudioProcessor&);
    ~ReverbAudioProcessorEditor() override;
    void paint(juce::Graphics&) override {}
    void resized() override;

private:
    ReverbAudioProcessor& audioProcessor;

    BgImage bgImage;
    PowerLine powerLine{ "Reverb", "Jacob Curtis", 30 };
    BigKnob sizeKnob{ "Size" };
    SmallKnob dampingKnob{ "Damping" },
        mixKnob{ "Mix", "%" },
        predelayKnob{ "Predelay", "ms" },
        depthKnob{ "Depth" },
        rateKnob{ "Rate", "Hz" },
        hpfKnob{ "HPF", "Hz" },
        lpfKnob{ "LPF", "Hz" };
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sizeAttach, dampingAttach,
        mixAttach, predelayAttach, depthAttach, rateAttach, hpfAttach, lpfAttach;
    MultiLabel filterLabel{ "Filters" }, modLabel{ "Mod" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReverbAudioProcessorEditor)
};
