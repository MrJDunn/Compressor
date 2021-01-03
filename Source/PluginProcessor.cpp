/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CompressorAudioProcessor::CompressorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
	parameters(*this, nullptr, Identifier("Compressor"), {
			std::make_unique<AudioParameterFloat> ("attack", "Attack", 0.0f, 1.0f, 0.5f),
			std::make_unique<AudioParameterFloat> ("release", "Release", 0.0f, 1.0f, 0.5f),
			std::make_unique<AudioParameterFloat> ("ratio", "Ratio", 0.0f, 1.0f, 0.5f),
			std::make_unique<AudioParameterFloat> ("threshold", "Threshold", -0.99f, 0.99f, 0.0f)
		})
#endif
{
}

CompressorAudioProcessor::~CompressorAudioProcessor()
{
}

//==============================================================================
const String CompressorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

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

double CompressorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CompressorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int CompressorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CompressorAudioProcessor::setCurrentProgram (int index)
{
}

const String CompressorAudioProcessor::getProgramName (int index)
{
    return {};
}

void CompressorAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void CompressorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void CompressorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CompressorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void CompressorAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;

	//readGain(buffer);

	float attack = (float)parameters.getParameterAsValue("attack").getValue() * 10000.f;
	float release = (float)parameters.getParameterAsValue("release").getValue() * 10000.f;
	float ratio = (float)parameters.getParameterAsValue("ratio").getValue() * 10.f;
	float threshold = 20 * log10((float)parameters.getParameterAsValue("threshold").getValue());

	compressor.setAttack(attack);
	compressor.setRelease(release);
	compressor.setRatio(ratio);
	compressor.setThreshold(threshold);
	
	compressor.process(buffer);
	//performCompression(buffer);
}

//==============================================================================
bool CompressorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* CompressorAudioProcessor::createEditor()
{
    return new CompressorAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void CompressorAudioProcessor::getStateInformation (MemoryBlock& destData)
{
	auto state = parameters.copyState();
	std::unique_ptr<juce::XmlElement> xml(state.createXml());
	copyXmlToBinary(*xml, destData);
}

void CompressorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
	std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

	if (xmlState.get() != nullptr)
		if (xmlState->hasTagName(parameters.state.getType()))
			parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

float CompressorAudioProcessor::getGain()
{
	return compressorState.gain;
}

void CompressorAudioProcessor::performCompression(AudioBuffer<float>& buffer)
{
	int numChannels = buffer.getNumChannels();
	int numSamples = buffer.getNumSamples();

	float attack = (float) parameters.getParameterAsValue("attack").getValue() * 10000.f;
	float release = (float)parameters.getParameterAsValue("release").getValue() * 10000.f;
	float ratio = (float)parameters.getParameterAsValue("ratio").getValue() * 10.f ;
	float threshold = (float)parameters.getParameterAsValue("threshold").getValue();

	readGain(buffer);

	switch(compressorState.mode)
	{
	case CompressorState::Mode::IDLE:
		if (compressorState.gain > threshold)
		{
			compressorState.time = Time::currentTimeMillis();
			compressorState.mode = CompressorState::Mode::ATTACKING;
		}
		break;
	case CompressorState::Mode::ATTACKING:
		if (Time::currentTimeMillis() - compressorState.time >= attack)
		{
			compressorState.time = Time::currentTimeMillis();
			compressorState.mode = CompressorState::Mode::RELEASING;
		}
		break;
	case CompressorState::Mode::RELEASING:
		buffer.applyGain(ratio / -1.f);
		if (Time::currentTimeMillis() - compressorState.time >= release)
		{
			compressorState.time = 0.0;
			compressorState.mode = CompressorState::Mode::IDLE;
		}
		break;
	default:
		break;
	}
}

void CompressorAudioProcessor::readGain(AudioBuffer<float>& buffer)
{
	auto numChannels = buffer.getNumChannels();
	auto numSamples = buffer.getNumSamples();

	compressorState.gain = 0.0f;

	for(int i = 0; i < numChannels; ++i)
		compressorState.gain += buffer.getRMSLevel(i, 0, numSamples);

	compressorState.gain = compressorState.gain / (float)numSamples;
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CompressorAudioProcessor();
}
