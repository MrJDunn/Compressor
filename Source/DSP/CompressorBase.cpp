#include "CompressorBase.h"

CompressorBase::CompressorBase(): threshold(-40), ratio(4), attack(1), release(1)
{
}

CompressorBase::~CompressorBase()
{
}

void CompressorBase::setup(double sampleRate, int blockSize)
{
	this->sampleRate = sampleRate;
	this->blockSize = blockSize;
}

void CompressorBase::process(AudioBuffer<float>& buffer)
{
	int channels = buffer.getNumChannels();
	int bufferSize = buffer.getNumSamples();

	for(int i = 0; i < channels; ++i)
	{
		float* channelData = buffer.getWritePointer(i);
		for(int j = 0; j < bufferSize; ++j)
		{
			// Calculate absolute db value of sample
			float unipolarInput = abs(channelData[j]);
			
			float sampleDb = 20 * log10(unipolarInput/1.f);
			if(sampleDb < minimumDb)
			{
				sampleDb = minimumDb;
			}


			float gain = sampleDb;
			if(sampleDb > threshold)
			{
				gain =  threshold + ((sampleDb - threshold) / ratio);
			}

			float reductionAmount = sampleDb - gain;

			float gainSmooth = 0.f;
			if(reductionAmount < gainSmoothPrevious)
			{
				// Attack
				attackAlpha =  pow(MathConstants<float>::euler, (-log(10)/ (sampleRate * attack / 5.f)));
				gainSmooth = (1.f - attackAlpha) * reductionAmount + (attackAlpha * gainSmoothPrevious);
			}
			else
			{
				// Release
				releaseAlpha = pow(MathConstants<float>::euler, (-log(10) / (sampleRate * release / 5.f)));
				gainSmooth = (1.f - releaseAlpha) * reductionAmount + (releaseAlpha * gainSmoothPrevious);
			}

			float newSampleValue = powf(10.f, (gainSmooth / 20.f)) * channelData[i];

			// TODO: Get to the root of why this is leaving the range at all
			//newSampleValue = std::max(-1.f,std::min(1.f,newSampleValue));
		//	jassert(newSampleValue <= 1.f && newSampleValue >= -1.f);
			channelData[i] = newSampleValue;
			
			gainSmoothPrevious = gainSmooth;

		}
	}
}

void CompressorBase::setAttack(float val)
{
	attack = std::min(5.f,std::max(1.f, val * 5.f));
}

void CompressorBase::setRelease(float val)
{
	release = std::min(5.f,std::max(1.f, val * 5.f));
}

void CompressorBase::setRatio(float val)
{
	ratio = std::min(16.f,std::max(1.f, val));
}

void CompressorBase::setThreshold(float val)
{
	threshold = std::min(0.f, std::max(minimumDb, val));
}

float CompressorBase::getAttack()
{
	return attack;
}

float CompressorBase::getRelease()
{
	return release;
}

float CompressorBase::getRatio()
{
	return ratio;
}

float CompressorBase::getThreshold()
{
	return threshold;
}

