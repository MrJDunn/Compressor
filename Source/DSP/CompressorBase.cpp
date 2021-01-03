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
			float sampleDb = 20 * log10(abs(channelData[j]));
			if(sampleDb < minimumDb)
			{
				sampleDb = minimumDb;
			}

			// If we are beyond our threshold, compress signal by ratio
			if(sampleDb > threshold)
			{
				channelData[i] = threshold + (sampleDb / ratio);
			}
		}
	}
}

void CompressorBase::setAttack(float val)
{
	attack = val;
}

void CompressorBase::setRelease(float val)
{
	release = val;
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
