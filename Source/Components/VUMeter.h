/*
  ==============================================================================

    VUMeter.h
    Created: 4 Oct 2020 6:37:13pm
    Author:  Jeff

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "../Utility/Converter.h"

#include "../Source/PluginProcessor.h"

class VUMeter : public AnimatedAppComponent
{
public: 
	VUMeter(CompressorAudioProcessor&);
	~VUMeter();

	void update() override;

	void resized() override;
	void paint(Graphics& g);
	
private:	
	//============================================================================
	CompressorAudioProcessor& processor;

	float needleValue = 0.0f;

	const float xOffset = 5.0f;
	const float yOffset = 10.0f;

	const float bevelDepthX = 15.0f;
	const float bevelDepthY = 10.0f;
	float defaultAngle = 0.f;
	float radianMultiplier = 0.f;

	//============================================================================
	void drawBorder(Graphics& g);
	void drawBackground(Graphics& g);
	void drawWindows(Graphics& g);
	void drawDial(Graphics& g);
	void drawNeedle(Graphics& g);

};