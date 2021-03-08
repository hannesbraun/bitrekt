#include "Downsampler.hpp"

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
  this->sampleLen = hostSampleRate / 44100;
  this->sampleLen *= 44100.0 / targetSampleRate;
  if (sampleLen < 1.0)
    sampleLen = 1.0;
  this->tts = samplePosition - this->sampleLen * ((int)(samplePosition / this->sampleLen));
}

double Downsampler::resample(double sample, int channel)
{
  if (channel >= MAX_CHANNELS)
    // Only supporting two channels
    return sample;

  // Fix first sample at playback (maybe old last value stored)

  if (this->tts >= this->sampleLen)
  {
    double frac = this->tts - this->sampleLen;
    this->sampleOut[channel] = frac * sample + (1 - frac) * lastSampleIn[channel];
  }

  this->lastSampleIn[channel] = sample;
  return this->sampleOut[channel];
}

void Downsampler::nextSample()
{
  if (this->tts >= this->sampleLen)
    this->tts = this->tts - this->sampleLen;
  else
    this->tts++;
}
