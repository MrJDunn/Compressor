/*
  ==============================================================================

	Knob.cpp
	Created: 9 Oct 2020 4:48:49pm
	Author:  Jeff

  ==============================================================================
*/

#include "Knob.h"

Knob::Knob(const StringArray& v): values(v)
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

	// Draw metallic knob background
	Image metal(Image::ARGB, 210, 210, true);
	Graphics glassG(metal);
	File metalFile = File("C:\\Users\\Jeff\\Documents\\Assetts\\Blackway FX Kit (Kontakt)\\Assets\\PNG Filmstripes 128 frames\\knob-big-black-dull.png");
	Image metalTile = ImageCache::getFromFile(metalFile);
	glassG.setTiledImageFill(metalTile, 0, 0, 1.f);
	glassG.fillAll();

	g.drawImage(metal, area.toFloat(), false);

	// Draw line
	double rangeOfMotion = 270.0;
	double offsetAngle = 179.9;

	double value = getValue();
	if(snapping)
	{
		//TODO figure out a way to calculate this that isn't O(n)
		double interval = getMaximum() / double(values.size() - 1);
		double target = 0.f;
		
		while(target < value)
		{
			target += interval;
		}

		value = target;
	}

	double angle = (rangeOfMotion * value / getMaximum()) * (MathConstants<double>::pi / 180.0);
	float hypotenuse = circleArea.getWidth() / 2.f - 17.0f;

	float xTip = circleArea.getCentreX() + sin(offsetAngle + -1 * angle + MathConstants<float>::halfPi) * hypotenuse;
	float yTip = circleArea.getCentreY() + cos(offsetAngle + -1 * angle + MathConstants<float>::halfPi) * hypotenuse;

	float xStart = circleArea.getCentreX() + sin(offsetAngle + -1 * angle + MathConstants<float>::halfPi) * (hypotenuse / 6.f);
	float yStart = circleArea.getCentreY() + cos(offsetAngle + -1 * angle + MathConstants<float>::halfPi) * (hypotenuse / 6.f);

	Point<float> start(xStart, yStart);
	Point<float> tip(xTip, yTip);

	Path needle;
	needle.startNewSubPath(start);
	needle.lineTo(tip);

	g.setColour(Colour(30, 30, 30));
	g.setColour(Colour(200, 200, 200));

	g.strokePath(needle,PathStrokeType(2.0f, PathStrokeType::JointStyle::mitered,PathStrokeType::EndCapStyle::rounded));

	// Draw numbers
	auto radius = circleArea.getWidth() / 2.f;

	for(int i = 0; i < values.size(); ++i)
	{
		double vAngle = (rangeOfMotion * i / ((float)values.size() - 1.f)) * (MathConstants<double>::pi / 180.0);

		float xPos = circleArea.getCentreX() + sin(offsetAngle + -1 * vAngle + MathConstants<float>::halfPi) * radius;
		float yPos = circleArea.getCentreY() + cos(offsetAngle + -1 * vAngle + MathConstants<float>::halfPi) * radius;

		Rectangle<float> position(xPos, yPos, 2.f,2.f);
		g.fillEllipse(position);

		float additionalSizeForText = 7.5f;
		float textSize = 10.f;
		float xPosText = circleArea.getCentreX() + sin(offsetAngle + -1 * vAngle + MathConstants<float>::halfPi) * (radius + additionalSizeForText);
		float yPosText = circleArea.getCentreY() + cos(offsetAngle + -1 * vAngle + MathConstants<float>::halfPi) * (radius + additionalSizeForText);

		Rectangle<float> textPosition(xPosText, yPosText, textSize, textSize / 2.f);

		Font txtFont(textSize);
		g.setFont(txtFont);
		g.drawText(values[i], textPosition, Justification::left);
	}
}

void Knob::setSnapping(bool val)
{
	snapping = val;
}
