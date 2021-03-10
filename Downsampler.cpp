#include "Downsampler.hpp"

#define FADEIN 0.1

Downsampler::Downsampler()
{
  for (int i = 0; i <= MAX_CHANNELS; i++)
  {
    this->lastSampleIn[i] = 0.0;
    this->sampleOut[i] = 0.0;
  }
}

void Downsampler::configure(double targetSampleRate, double hostSampleRate, double samplePosition)
{
  this->sampleLen = hostSampleRate / BASE_SAMPLE_RATE;
  this->sampleLen *= BASE_SAMPLE_RATE / targetSampleRate;
  if (sampleLen < 1.0)
    // Avoid oversampling
    sampleLen = 1.0;

  this->tts = samplePosition - this->sampleLen * ((int)(samplePosition / this->sampleLen));

  // Fade in the effect in the highest 10% (FADEIN) to allow turning this effect completely off
  const double fadeinRange = BASE_SAMPLE_RATE - BASE_SAMPLE_RATE * FADEIN;
  this->wetness = (BASE_SAMPLE_RATE - targetSampleRate) / fadeinRange;
  if (this->wetness > 1.0)
    this->wetness = 1.0;
}

double Downsampler::resample(double sample, int channel)
{
  if (channel >= MAX_CHANNELS)
    // Only supporting two channels
    return sample;

  if (this->tts >= this->sampleLen)
  {
    // It's time to sample!
    double frac = this->tts - this->sampleLen;
    this->sampleOut[channel] = frac * sample + (1 - frac) * lastSampleIn[channel];
  }

  this->lastSampleIn[channel] = sample;
  return this->wetness * this->sampleOut[channel] + (1.0 - this->wetness) * sample;
}

void Downsampler::nextSample()
{
  if (this->tts >= this->sampleLen)
    this->tts = this->tts - this->sampleLen;
  else
    this->tts++;
}
