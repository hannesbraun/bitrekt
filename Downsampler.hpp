#ifndef Downsampler_hpp
#define Downsampler_hpp

#define MAX_CHANNELS 2
#define BASE_SAMPLE_RATE 44100.0

class Downsampler
{
public:
  Downsampler();
  void configure(double targetSampleRate, double hostSampleRate, double samplePosition);
  double resample(double sample, int channel);
  void nextSample();
private:
  double sampleLen; // Source sample length (how long to keep a sample from the source)
  double tts; // Time to sample -> Time elapsed since last sample change
  double lastSampleIn[MAX_CHANNELS]; // Last source sample
  double sampleOut[MAX_CHANNELS]; // Current sample to output
  double wetness; // 1.0 means full wetness, 0.0 is the dry input signal
};

#endif /* Downsampler_hpp */
