/*
  ==============================================================================

    VUMeter.cpp
    Created: 4 Oct 2020 6:37:13pm
    Author:  Jeff

  ==============================================================================
*/

#include "VUMeter.h"

VUMeter::VUMeter(CompressorAudioProcessor& p): processor(p)
{
	setOpaque(false);
	setFramesPerSecond(30);
}

VUMeter::~VUMeter()
{
}

void VUMeter::update()
{
	needleValue = processor.getGain();
	repaint();
}

void VUMeter::resized()
{
	repaint();
}

void VUMeter::paint(Graphics& g)
{
	drawBackground(g);
	drawDial(g);
	drawNeedle(g);
	drawBorder(g);
	drawWindows(g);
}

/*
   nwo                            neo
	-------------------------------
	|\---------------------------/|
	|| nwi                   nei ||
	||                           ||
	||                           ||
	||                           ||
	|| swi                   sei ||
	|/---------------------------\|
	-------------------------------
   swo                            seo
*/
void VUMeter::drawBorder(Graphics& g)
{
	const float width = (float)getWidth();
	const float height = (float)getHeight();

	Point<float> nwo(0.0f + xOffset, 0.0f + yOffset);
	Point<float> neo(width - xOffset, 0.0f + yOffset);
	Point<float> seo(width - xOffset, height - yOffset);
	Point<float> swo(0.0f + xOffset, height - yOffset);

	Point<float> nwi(0.0f + bevelDepthX + xOffset, 0.0f + bevelDepthY + yOffset);
	Point<float> nei(width - bevelDepthX - xOffset, 0.0f + bevelDepthY + yOffset);
	Point<float> sei(width - bevelDepthX - xOffset, height - bevelDepthY - yOffset);
	Point<float> swi(0.0f + bevelDepthX + xOffset, height - bevelDepthY - yOffset);

	Path topBevel;
	topBevel.startNewSubPath(nwo);
	topBevel.lineTo(neo);
	topBevel.lineTo(nei);
	topBevel.lineTo(nwi);
	topBevel.closeSubPath();

	g.setColour(Colour::fromRGB(45, 49, 55));
	g.fillPath(topBevel);

	Path rightBevel;
	rightBevel.startNewSubPath(neo);
	rightBevel.lineTo(seo);
	rightBevel.lineTo(sei);
	rightBevel.lineTo(nei);
	rightBevel.closeSubPath();

	g.setColour(Colour::fromRGB(69, 73, 78));
	g.fillPath(rightBevel);

	Path bottomBevel;
	bottomBevel.startNewSubPath(seo);
	bottomBevel.lineTo(swo);
	bottomBevel.lineTo(swi);
	bottomBevel.lineTo(sei);
	bottomBevel.closeSubPath();

	g.setColour(Colour::fromRGB(116, 121, 127));
	g.fillPath(bottomBevel);

	Path leftBevel;
	leftBevel.startNewSubPath(swo);
	leftBevel.lineTo(nwo);
	leftBevel.lineTo(nwi);
	leftBevel.lineTo(swi);
	leftBevel.closeSubPath();

	g.setColour(Colour::fromRGB(69, 73, 78));
	g.fillPath(leftBevel);

	DropShadow shadow;

	const int shadowDepth = 2;
	juce::Rectangle<int> nShadow((int)nwi.getX(), (int)nwi.getY() - shadowDepth, (int)(nei.getX() - nwi.getX()), shadowDepth);
	juce::Rectangle<int> eShadow((int)nei.getX(), (int)nei.getY(), shadowDepth, (int)(sei.getY() - nei.getY()));
	juce::Rectangle<int> sShadow((int)swi.getX(), (int)swi.getY() - shadowDepth, (int)(sei.getX() - swi.getX()), shadowDepth);
	juce::Rectangle<int> wShadow((int)nwi.getX() - shadowDepth, (int)nwi.getY(), shadowDepth, (int)(swi.getY() - nwi.getY()));

	shadow.drawForRectangle(g, nShadow);
	shadow.drawForRectangle(g, eShadow);
	shadow.drawForRectangle(g, sShadow);
	shadow.drawForRectangle(g, wShadow);

}

void VUMeter::drawBackground(Graphics & g)
{
	// Exterior background
	auto area = getLocalBounds();
	auto rec = convertRectangle<int, float>(area);
	auto curveDepth = 10.0f;

	g.setColour(Colour(53, 57, 62));
	g.fillRoundedRectangle(rec->reduced(1), curveDepth);

	g.setColour(Colour(23, 27, 30));
	g.drawRoundedRectangle(*rec, curveDepth, 0.1f);

	// Display background
	const float width = (float)getWidth();
	const float height = (float)getHeight();

	Point<float> nwi(0.0f + bevelDepthX + xOffset, 0.0f + bevelDepthY + yOffset);
	Point<float> sei(width - bevelDepthX - xOffset, height - bevelDepthY - yOffset);

	juce::Rectangle<float> centerRectangle(nwi, sei);

	auto centerRectangleCurveDepth = 5.0f;

	g.setColour(Colour(168, 178, 188));
	g.fillRoundedRectangle(centerRectangle.reduced(2), centerRectangleCurveDepth);

	delete rec;
}

void VUMeter::drawWindows(Graphics& g)
{
	const float width = (float)getWidth();
	const float height = (float)getHeight();

	Point<float> nwi(0.0f + bevelDepthX + xOffset, 0.0f + bevelDepthY + yOffset);
	Point<float> sei(width - bevelDepthX - xOffset, height - bevelDepthY - yOffset);

	juce::Rectangle<float> centerRectangle(nwi, sei);

	Image glass(Image::ARGB, 736, 409, true);
	Graphics glassG(glass);

	File glassFile = File("C:\\Users\\Jeff\\Pictures\\Textures\\Glass.png");
	Image glassTile = ImageCache::getFromFile(glassFile);

	glassG.setTiledImageFill(glassTile, centerRectangle.getX(), centerRectangle.getY(), 0.5f);
	glassG.fillAll();

	g.drawImage(glass, centerRectangle, false);
}

void VUMeter::drawDial(Graphics& g)
{
	Point<float> leftCurve(25.0f, getHeight() / 2.0f);
	Point<float> rightCurve(getWidth() - 25.0f, getHeight() / 2.0f);

	g.setColour(Colours::black);
	Path curve;
	curve.addArc((float)leftCurve.getX(), (float)leftCurve.getY(), (float)rightCurve.getX() - leftCurve.getX(), 40.0f, 1.6 * MathConstants<float>::pi, 2.4f * MathConstants<float>::pi, true);
	g.strokePath(curve, PathStrokeType(1.3f));
}


void VUMeter::drawNeedle(Graphics& g)
{
	g.setColour(Colour(53,57,62));

	Point<float> start(getWidth() / 2.0f - 2.0f, getHeight() * 1.33f);
	Point<float> tip((float)1000 * needleValue * getWidth(), 50.0f);
	Point<float> end(getWidth() / 2.0f + 2.0f, getHeight() * 1.33f);

	Path needle;
	needle.startNewSubPath(start);
	needle.lineTo(tip);
	needle.lineTo(end);
	needle.closeSubPath();

	g.fillPath(needle);
}
