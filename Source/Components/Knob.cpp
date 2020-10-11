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

	g.fillEllipse(circleArea);

	Image metal(Image::ARGB, 1024, 1024, true);
	Graphics glassG(metal);

	File metalFile = File("C:\\Users\\Jeff\\Pictures\\Textures\\metal3.png");
	Image metalTile = ImageCache::getFromFile(metalFile);

	glassG.setTiledImageFill(metalTile, 0, 0, 1.0f);
	glassG.fillAll();

	g.drawImage(metal, area.toFloat(), false);

	g.setColour(Colour(53, 57, 62));

	g.fillEllipse(circleArea.reduced(8));

}
