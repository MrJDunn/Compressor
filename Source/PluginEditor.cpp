/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CompressorAudioProcessorEditor::CompressorAudioProcessorEditor(CompressorAudioProcessor& p, AudioProcessorValueTreeState& vts)
	: AudioProcessorEditor(&p), 
	processor(p), 
	vuMeter(p), 
	valueTree(vts),
	attackAttachment(vts, "attack", sAttack),
	releaseAttachment(vts, "release", sRelease),
	ratioAttachment(vts, "ratio", sRatio),
	thresholdAttachment(vts, "threshold", sThreshold),
	sAttack({ "100", "", "300", "", "500", "", "700", "", "900" }),
	sRelease({ "100", "", "300", "", "500", "", "700", "", "900" }),
	sRatio({ "1:1","4:1","8:1","12:1","16:1" }),
	sThreshold({ "-144","","-108","","-72","","-36","","0" })
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (270, 450);

	File stencil("C:\\Users\\Jeff\\Pictures\\CompressorTemplate.png");
	if(stencil.existsAsFile())
		iStencil = ImageFileFormat::loadFrom(stencil);

	addAndMakeVisible(vuMeter);

	setupSlider(sAttack, lAttack, "Attack ms");
	setupSlider(sRelease, lRelease, "Release ms");
	setupSlider(sRatio, lRatio, "Ratio");
	setupSlider(sThreshold, lThreshold, "Threshold db");

	sThreshold.setSnapping(false);
	sAttack.setSnapping(false);
	sRelease.setSnapping(false);
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

	g.setColour(Colour::fromRGBA(10,20,10,100));
	g.drawRoundedRectangle({ 20, 177, 230, 250 }, 15.f, 1.f);

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

	const int KNOB_HEIGHT = 100;
	const int LABEL_HEIGHT = 40;

	auto knobRow1 = area.removeFromTop(KNOB_HEIGHT);
	auto labelRow1 = area.removeFromTop(LABEL_HEIGHT);
	auto knobRow2 = area.removeFromTop(KNOB_HEIGHT);
	auto labelRow2 = area.removeFromTop(LABEL_HEIGHT);

	sAttack.setBounds(knobRow1.removeFromLeft(knobRow1.getWidth()/2.0f));
	sRelease.setBounds(knobRow1.removeFromLeft(knobRow1.getWidth()));

	lAttack.setBounds(labelRow1.removeFromLeft(labelRow1.getWidth() / 2.0f));
	lRelease.setBounds(labelRow1.removeFromLeft(labelRow1.getWidth()));

	sRatio.setBounds(knobRow2.removeFromLeft(knobRow2.getWidth() / 2.0f));
	sThreshold.setBounds(knobRow2.removeFromLeft(knobRow2.getWidth()));

	lRatio.setBounds(labelRow2.removeFromLeft(labelRow2.getWidth() / 2.0f));
	lThreshold.setBounds(labelRow2.removeFromLeft(labelRow2.getWidth()));
}

void CompressorAudioProcessorEditor::setupSlider(Slider& slider, Label& label, const String& text)
{
	addAndMakeVisible(label);

	Font lblFont(10.0f);
	label.setFont(lblFont);
	label.setText(text, dontSendNotification);
	label.setJustificationType(Justification::centredTop);

	addAndMakeVisible(slider);
	slider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	slider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
}
