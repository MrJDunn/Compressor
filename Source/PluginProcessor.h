/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../Compressor/Source/DSP/CompressorBase.h"

//==============================================================================
/**
*/
class CompressorAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    CompressorAudioProcessor();
    ~CompressorAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

	float getGain();
private:

	AudioProcessorValueTreeState parameters;

	CompressorBase compressor;

	struct CompressorState
	{
		enum Mode { IDLE = 0, ATTACKING, RELEASING} mode = Mode::IDLE;
		float gain = 0.0f;
		double time = 0.0;
	} compressorState;

	void performCompression(AudioBuffer<float>&);

	void readGain(AudioBuffer<float>&);

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CompressorAudioProcessor)
};
