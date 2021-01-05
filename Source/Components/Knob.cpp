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

	g.drawImage(metal, area.reduced(12.f).toFloat(), false);

	// Draw line
	double rangeOfMotion = 270.0;
	double offsetAngle = 179.9;

	double value = getValue();
	double max = getMaximum();

	if(snapping)
	{
		//TODO figure out a way to calculate this that isn't O(n)
		double interval = max / double(values.size() - 1);
		double target = 0.f;
		
		while(target < value)
		{
			target += interval;
		}

		value = target;
	}

	double angle = (rangeOfMotion * value / max) * (MathConstants<double>::pi / 180.0);
	float hypotenuse = circleArea.getWidth() / 2.f - 28.0f;

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
		
		Line<float> line({float(area.getCentreX()), float(area.getCentreY()) }, { xPos, yPos });

		// Draw dot
		Line<float> dotLine = line.withShortenedEnd(12.f);
		Point<float> topLeftDot = dotLine.getEnd();
		Point<float> bottomRightDot = dotLine.getEnd();
		topLeftDot.addXY(-1.5f,-1.5f);
		bottomRightDot.addXY(1.5f,1.5f);
		Rectangle<float> dotPosition(topLeftDot,bottomRightDot);

		g.fillEllipse(dotPosition);

		// Draw text
		float additionalSizeForText = 7.5f;
		float textSize = 8.f;

		Line<float> textLine = line.withShortenedEnd(2.f);
		Point<float> topLeftText = textLine.getEnd();
		Point<float> bottomRightText = textLine.getEnd();
		topLeftText.addXY(-10.f,-10.f);
		bottomRightText.addXY(10.f,10.f);
		Rectangle<float> textPosition(topLeftText,bottomRightText);
		
		Font txtFont(textSize);
		g.setFont(txtFont);
		g.drawText(values[i], textPosition, Justification::centred);
	
	}
}

void Knob::setSnapping(bool val)
{
	snapping = val;
}
