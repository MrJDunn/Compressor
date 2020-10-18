/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

#include "Components/VUMeter.h"
#include "Components/Knob.h"

#include "Utility/Converter.h"

//==============================================================================
/**
*/
class CompressorAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    CompressorAudioProcessorEditor (CompressorAudioProcessor&, AudioProcessorValueTreeState&);
    ~CompressorAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
	void paintOverChildren (Graphics&) override;

    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    CompressorAudioProcessor& processor;

	Image iStencil;
	VUMeter vuMeter;

	Slider sAttack;
	Knob sRelease;
	Knob sRatio;
	Knob sThreshold;

	Label lAttack;
	Label lRelease;
	Label lRatio;
	Label lThreshold;

	AudioProcessorValueTreeState::SliderAttachment attackAttachment;
	AudioProcessorValueTreeState::SliderAttachment releaseAttachment;
	AudioProcessorValueTreeState::SliderAttachment ratioAttachment;
	AudioProcessorValueTreeState::SliderAttachment thresholdAttachment;

	AudioProcessorValueTreeState& valueTree;

	void setupSlider(Slider&, Label&, const String&);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CompressorAudioProcessorEditor)
};
