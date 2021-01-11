/**
	Simple hard knee (no knee implemented) compressor
*/

#pragma once
#include <JuceHeader.h>

class CompressorBase
{
public:
	CompressorBase();
	virtual ~CompressorBase();

	virtual void setup(double sampleRate, int blockSize);
	virtual void process(AudioBuffer<float>&);

	void setAttack(float);
	void setRelease(float);
	void setRatio(float);
	void setThreshold(float);
		
	float getAttack();
	float getRelease();
	float getRatio();
	float getThreshold();

	float getCompressionAmountDB();

protected:
	/**
		0 < attack < 5
	*/
	float attack;

	/**
		0 < release < 5
	*/
	float release;

	/**
		2 < ratio < 16
	*/
	float ratio;

	/** 
		-96 < threshold < 0
	*/
	float threshold;

private:
	float attackAlpha = 0.99f;
	float releaseAlpha = 0.99f;

	float gainSmoothPrevious = 0.0f;

	double sampleRate = 44100.0;
	int blockSize = 512;
	float reductionAmount = 0.f;
	float minimumDb = -40.f;
	std::atomic<float> lastGreatestReductionAmount = 0.f;
};

