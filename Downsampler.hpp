#ifndef Downsampler_hpp
#define Downsampler_hpp

#define MAX_CHANNELS 2

class Downsampler
{
public:
  Downsampler();
  void configure(double targetSampleRate, double hostSampleRate, double samplePosition);
  double resample(double sample, int channel);
  void nextSample();
private:
  double sampleLen;
  double tts; // Time to sample
  double lastSampleIn[MAX_CHANNELS];
  double sampleOut[MAX_CHANNELS];
};

#endif /* Downsampler_hpp */
