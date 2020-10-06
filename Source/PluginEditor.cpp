/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CompressorAudioProcessorEditor::CompressorAudioProcessorEditor (CompressorAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), vuMeter(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (270, 450);

	File stencil("C:\\Users\\Jeff\\Pictures\\CompressorTemplate.png");
	if(stencil.existsAsFile())
		iStencil = ImageFileFormat::loadFrom(stencil);

	addAndMakeVisible(vuMeter);
}

CompressorAudioProcessorEditor::~CompressorAudioProcessorEditor()
{
}

//==============================================================================
void CompressorAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)


    g.fillAll();

	// Noise mask courtesy of P4tr3ck https://forum.juce.com/t/background-texture-drawing/11223/3
	g.setGradientFill(ColourGradient(
		Colour(45, 51, 57), juce::Point<float>(getWidth() / 2, getHeight() / 2),
		Colour(27, 28, 30), juce::Point<float>(getWidth(), getHeight()),
		true
	));
	g.fillAll();

	Image noise(Image::ARGB, getWidth(), getHeight(), true);
	Graphics noiseG(noise);

	AffineTransform t;

	File noiseFile = File("C:\\Users\\Jeff\\Pictures\\Textures\\SoftNoise.png");
	Image noiseTile = ImageCache::getFromFile(noiseFile);

	noiseG.setTiledImageFill(noiseTile, 0, 0, 0.0175f);
	noiseG.fillAll();

	g.drawImageAt(noise, 0, 0, false);
	
	//auto rec = convertRectangle<int, float>(getLocalBounds());
	//g.drawImage(iStencil, *rec, RectanglePlacement::centred);
	//delete rec;
}

void CompressorAudioProcessorEditor::paintOverChildren(Graphics& g)
{
	// Reflection from glass
	const int lightX = getWidth() - 70;
	const int lightY = 70;

	const int lightRadius = 33;
	
	g.setGradientFill(ColourGradient(Colours::white.withAlpha(0.25f), lightX, lightY, Colours::transparentWhite, lightX + lightRadius, lightY + lightRadius, true));

	g.fillAll();

	// Light effect
	//const int lightX = getWidth() - 50;
	//const int lightY = 50;

	//const int lightRadius = 150;

	//g.setGradientFill(ColourGradient(Colours::white.withAlpha(0.2f), lightX, lightY, Colours::transparentWhite, lightRadius, lightRadius, true));

	//g.fillAll();
}

void CompressorAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
	auto area = getLocalBounds();

	vuMeter.setBounds(area.removeFromTop(190).reduced(20));
}
