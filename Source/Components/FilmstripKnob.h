/*
  ==============================================================================

    FilmstripKnob.h
    Created: 15 Oct 2020 8:17:48pm
    Author:  Jeff

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class FilmstripKnob : public Slider
{
public:
	FilmstripKnob(Image);
	~FilmstripKnob();
	
	void setImage(Image);
	void resized() override;
	void paint(Graphics& g) override;
private:

	Image image;
	float imageWidth = 0.f;
	size_t frameCount = 0;
};