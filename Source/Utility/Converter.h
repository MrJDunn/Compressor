/*
  ==============================================================================

    Converter.h
    Created: 4 Oct 2020 6:45:55pm
    Author:  Jeff

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

template<class T1, class T2>
Rectangle<T2>* convertRectangle(juce::Rectangle<T1> rectangle)
{
	return new juce::Rectangle<T2>((T2)rectangle.getX(), (T2)rectangle.getY(), 
		(T2)rectangle.getWidth(), (T2)rectangle.getHeight());
}