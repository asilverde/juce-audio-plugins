/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

CompressorAudioProcessorEditor::CompressorAudioProcessorEditor(CompressorAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p), grMeter(audioProcessor.gainReduction)
{
    addAndMakeVisible(bgImage);
    addAndMakeVisible(thresholdKnob);
    addAndMakeVisible(attackKnob);
    addAndMakeVisible(releaseKnob);
    addAndMakeVisible(ratioKnob);
    addAndMakeVisible(makeUpKnob);
    addAndMakeVisible(scFreqKnob);
    addAndMakeVisible(mixKnob);
    addAndMakeVisible(scBypassButton);
    addAndMakeVisible(stereoButton);
    addAndMakeVisible(grMeter);
    thresholdAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters, "threshold", thresholdKnob);
    attackAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters, "attack", attackKnob);
    releaseAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters, "release", releaseKnob);
    ratioAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters, "ratio", ratioKnob);
    makeUpAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters, "makeUp", makeUpKnob);
    scFreqAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters, "scFreq", scFreqKnob);
    scBypassAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.parameters, "scBypass", scBypassButton);
    stereoAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.parameters, "stereo", stereoButton);
    mixAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters, "mix", mixKnob);
    setSize(280, 420);
}

CompressorAudioProcessorEditor::~CompressorAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

void CompressorAudioProcessorEditor::resized()
{
    bgImage.setBounds(getLocalBounds());
    const int col1XPosition = 20;
    const int col2XPosition = 110;
    const int col3XPosition = 200;
    const int sliderWidth = 50;
    // col 1
    thresholdKnob.setBounds(col1XPosition, 60, sliderWidth, sliderWidth + 25);
    attackKnob.setBounds(col1XPosition, 160, sliderWidth, sliderWidth + 25);
    makeUpKnob.setBounds(col1XPosition, 260, sliderWidth, sliderWidth + 25);
    stereoButton.setBounds(col1XPosition, 360, sliderWidth, sliderWidth);
    // col 2
    ratioKnob.setBounds(col2XPosition, 60, sliderWidth, sliderWidth + 25);
    releaseKnob.setBounds(col2XPosition, 160, sliderWidth, sliderWidth + 25);
    scFreqKnob.setBounds(col2XPosition, 260, sliderWidth, sliderWidth + 25);
    scBypassButton.setBounds(col2XPosition, 360, sliderWidth, sliderWidth);
    // col 3
    grMeter.setBounds(col3XPosition + 10, 70, grMeter.getMeterWidth(), grMeter.getMeterHeight());
    mixKnob.setBounds(col3XPosition + 4, 332, sliderWidth, sliderWidth + 25);
}
