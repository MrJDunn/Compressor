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

	attackAlpha = exp(-1.f / (sampleRate * 0.001f * attack));
	releaseAlpha = exp(-1.f / (sampleRate * 0.001f * release));

	lastGreatestReductionAmount = 0.f;
	
	for (int i = 0; i < channels; ++i)
	{
		float* channelData = buffer.getWritePointer(i);
		for (int j = 0; j < bufferSize; ++j)
		{
			// Calculate db value of sample
			float unipolarInput = abs(channelData[j]);
			float inputDb = 20 * log10(unipolarInput);

			// Avoid negative infinity
			inputDb = std::max(inputDb, minimumDb);

			// Calculate new target gain if greater than threshold
			float outputDb = inputDb > threshold ? threshold + ((inputDb - threshold) / ratio) : inputDb;

			reductionAmount = inputDb - outputDb;

			lastGreatestReductionAmount = std::max(reductionAmount, lastGreatestReductionAmount);
		//	DBG(reductionAmount);

			float gainSmooth = 0.f;

			if (reductionAmount < gainSmoothPrevious)
			{
				// Attack
				gainSmooth = (((1.f - attackAlpha) * reductionAmount) + (attackAlpha * gainSmoothPrevious));
			}
			else
			{
				// Release
				gainSmooth = (((1.f - releaseAlpha) * reductionAmount) + (releaseAlpha * gainSmoothPrevious));
			}

			float newSampleValue = powf(10.f, (gainSmooth / 20.f));

			//newSampleValue = std::max(-1.f,std::min(1.f,newSampleValue));
			//jassert(newSampleValue <= 1.f && newSampleValue >= -1.f);

			channelData[j] *= newSampleValue;

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

float CompressorBase::getCompressionAmountDB()
{
	return lastGreatestReductionAmount;
}

