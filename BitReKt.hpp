#pragma once

#include "IPlug_include_in_plug_hdr.h"

#include "BitCrusher.hpp"
#include "Downsampler.hpp"

const int kNumPresets = 1;

enum EParams
{
  kBits = 0,
  kSampleRate,
  kInputGain,
  kMix,
  kOutputGain,
  kClippingEnabled,
  kNumParams
};

enum EControlTags
{
  kCtrlTagSignalIn = 0,
  kCtrlTagOfflineRender,
  kCtrlTagClipping,
  kCtrlTags
};

using namespace iplug;
using namespace igraphics;

class BitReKt final : public Plugin
{
public:
  BitReKt(const InstanceInfo& info);
  void OnIdle() override;
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
private:
  BitCrusher bitCrusher;
  Downsampler downsampler;
  std::atomic<bool> clippingSignal;
};
