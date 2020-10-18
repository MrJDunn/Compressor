/*
  ==============================================================================

	Knob.cpp
	Created: 9 Oct 2020 4:48:49pm
	Author:  Jeff

  ==============================================================================
*/

#include "Knob.h"

Knob::Knob(const StringArray& values)
{
	setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	Range<double> range = Range<double>(1, values.size());
	setRange(range, 1);
	setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 25, 25);
}

Knob::~Knob()
{
}

void Knob::resized()
{
	Slider::resized();
}

void Knob::paint(Graphics & g)
{
	g.setColour(Colours::black);

	auto area = getLocalBounds();
	float x = area.getX();
	float y = area.getY();
	float width = area.getWidth();
	float height = area.getHeight();

	juce::Rectangle<float> circleArea = juce::Rectangle<float>(
		(width - x) / 2 - jmin(width, height) / 2.0, y,
		jmin(width, height),
		jmin(width, height));

	//g.fillEllipse(circleArea);

	Image metal(Image::ARGB, 210, 210, true);
	Graphics glassG(metal);
	File metalFile = File("C:\\Users\\Jeff\\Documents\\Assetts\\Blackway FX Kit (Kontakt)\\Assets\\PNG Filmstripes 128 frames\\knob-big-black-dull.png");
	Image metalTile = ImageCache::getFromFile(metalFile);
	glassG.setTiledImageFill(metalTile, 0, 0, 1.f);
	glassG.fillAll();

	g.drawImage(metal, area.toFloat(), false);

	g.setColour(Colour(53, 57, 62));

	double angle = (360.0 * getValue() / getMaximum()) * (MathConstants<double>::pi / 180.0);
	float hypotenuse = circleArea.getWidth() / 2.f - 15.0f;

	float xCoordinate = circleArea.getCentreX() + sin(angle - MathConstants<float>::halfPi) * hypotenuse;
	float yCoordinate = circleArea.getCentreY() + cos(angle - MathConstants<float>::halfPi) * hypotenuse;

	Point<float> start(circleArea.getX() + circleArea.getWidth() / 2.f, circleArea.getY() + circleArea.getHeight() / 2.f);
	Point<float> tip(xCoordinate, yCoordinate);

	Path needle;
	needle.startNewSubPath(start);
	needle.lineTo(tip);

	g.setColour(Colours::darkgrey);
	g.strokePath(needle,PathStrokeType(2.0f));

	//g.setColour(Colour(53, 57, 62));
	//g.fillEllipse(circleArea.reduced(8));

	// Noise on knob
	//Image noise(Image::ARGB, width, width, true);
	//Graphics noiseG(noise);

	//AffineTransform t;

	//File noiseFile = File("C:\\Users\\Jeff\\Pictures\\Textures\\SoftNoise.png");
	//Image noiseTile = ImageCache::getFromFile(noiseFile);

	//noiseG.setTiledImageFill(noiseTile, width, width, 0.0175f);
	//noiseG.fillEllipse(circleArea.reduced(8));


	//g.drawImageTransformed(noise, AffineTransform().rotated(-1.0f * getValue() * (360.0f / getMaximum(), width, width)));

	DBG("VALUE: " + String(getValue()));
}
