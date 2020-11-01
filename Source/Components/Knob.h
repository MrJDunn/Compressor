/*
  ==============================================================================

    Knob.h
    Created: 9 Oct 2020 4:48:49pm
    Author:  Jeff

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class Knob : public Slider
{
public:
	Knob(const StringArray&);
	~Knob();

	void resized() override;
	void paint(Graphics& g) override;
private:
	StringArray values;
};