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

	Image metal(Image::ARGB, 210, 210, true);
	Graphics glassG(metal);
	File metalFile = File("C:\\Users\\Jeff\\Documents\\Assetts\\Blackway FX Kit (Kontakt)\\Assets\\PNG Filmstripes 128 frames\\knob-big-black-dull.png");
	Image metalTile = ImageCache::getFromFile(metalFile);
	glassG.setTiledImageFill(metalTile, 0, 0, 1.f);
	glassG.fillAll();

	g.drawImage(metal, area.toFloat(), false);


	double rangeOfMotion = 270.0;
	double offsetAngle = 179.9;
	double angle = (rangeOfMotion * getValue() / getMaximum()) * (MathConstants<double>::pi / 180.0);
	float hypotenuse = circleArea.getWidth() / 2.f - 17.0f;

	float xCoordinate = circleArea.getCentreX() + sin(offsetAngle + -1 * angle + MathConstants<float>::halfPi) * hypotenuse;
	float yCoordinate = circleArea.getCentreY() + cos(offsetAngle + -1 * angle + MathConstants<float>::halfPi) * hypotenuse;

	Point<float> start(circleArea.getX() + circleArea.getWidth() / 2.f, circleArea.getY() + circleArea.getHeight() / 2.f);
	Point<float> tip(xCoordinate, yCoordinate);

	Path needle;
	needle.startNewSubPath(start);
	needle.lineTo(tip);

	g.setColour(Colour(30, 30, 30));
	g.strokePath(needle,PathStrokeType(2.0f, PathStrokeType::JointStyle::mitered,PathStrokeType::EndCapStyle::rounded));
}
