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


protected:
	float attack;

	float release;

	/**
		1 < ratio < 16
	*/
	float ratio;

	/** 
		-144 < threshold < 0
	*/
	float threshold;

private:
	double sampleRate = 44100.0;
	int blockSize = 512;

	float minimumDb = -144.f;
};

