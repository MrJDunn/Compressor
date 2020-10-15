/*
  ==============================================================================

    FilmstripKnob.cpp
    Created: 15 Oct 2020 8:17:49pm
    Author:  Jeff

  ==============================================================================
*/

#include "FilmstripKnob.h"

FilmstripKnob::FilmstripKnob(Image image)
{
	setImage(std::move(image));
	setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 25, 25);
}

FilmstripKnob::~FilmstripKnob()
{
}

/** Assign the filmstrip image.
   @param Image* newImage the filmstrip image
*/
void FilmstripKnob::setImage(Image newImage)
{
	image = newImage;

	imageWidth = (float) image.getWidth();
	frameCount = image.getHeight() / imageWidth;
}

void FilmstripKnob::resized()
{
	Slider::resized();
}

void FilmstripKnob::paint(Graphics & g)
{
	auto area = getLocalBounds().toFloat();

	size_t imageNumber = static_cast<std::size_t>(0.5 + (getValue() - getMinimum()) / (getMaximum() - getMinimum()) * (frameCount - 1));
	g.drawImage(image, 0, 0, imageWidth, imageWidth, 0, imageNumber * imageWidth, imageWidth, imageWidth);
}
