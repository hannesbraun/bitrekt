#include "BitReKt.hpp"
#include "IPlug_include_in_plug_src.h"
#include "IControls.h"

BitReKt::BitReKt(const InstanceInfo& info)
  : Plugin(info, MakeConfig(kNumParams, kNumPresets))
{
  GetParam(kBits)->InitDouble("Bits", 8.0, 1.0, 16.0, 0.01, "Bit");
  GetParam(kSampleRate)->InitFrequency("Sample Rate", BASE_SAMPLE_RATE, 100.0, BASE_SAMPLE_RATE, 0.01);
  GetParam(kInputGain)->InitDouble("Input Gain", 100.0, 0., 200.0, 0.01, "%");
  GetParam(kMix)->InitDouble("Dry/Wet", 100.0, 0., 100.0, 0.01, "%");
  GetParam(kOutputGain)->InitDouble("Output Gain", 100.0, 0., 200.0, 0.01, "%");
  GetParam(kClippingEnabled)->InitBool("Clipping", true, "", 0, "", "Enabled", "Disabled");

  mMakeGraphicsFunc = [&]()
  {
    return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, GetScaleForScreen(PLUG_WIDTH, PLUG_HEIGHT));
  };

  mLayoutFunc = [&](IGraphics * pGraphics)
  {
    pGraphics->AttachCornerResizer(EUIResizerMode::Scale, false);
    pGraphics->AttachPanelBackground(IColor(255, 213, 213, 221));
    pGraphics->LoadFont("Roboto-Regular", ROBOTO_FN);
    const IRECT b = pGraphics->GetBounds();
    pGraphics->AttachControl(new ITextControl(b.GetFromTop(100).GetCentredInside(PLUG_WIDTH, 60), "BitReKt", IText(50)));

    // Status LEDs
    pGraphics->AttachControl(new ILEDControl(b.GetReducedFromTop(90).GetFromTop(14).GetReducedFromRight(20).GetFromRight(20), COLOR_GREEN), kCtrlTagSignalIn);
    pGraphics->AttachControl(new ITextControl(b.GetReducedFromTop(90).GetFromTop(14).GetReducedFromRight(45).GetFromRight(39), "Signal In", IText(14)));
    pGraphics->AttachControl(new ILEDControl(b.GetReducedFromTop(110).GetFromTop(14).GetReducedFromRight(20).GetFromRight(20), COLOR_BLUE), kCtrlTagOfflineRender);
    pGraphics->AttachControl(new ITextControl(b.GetReducedFromTop(110).GetFromTop(14).GetReducedFromRight(45).GetFromRight(80), "Offline Rendering", IText(14)));
    pGraphics->AttachControl(new ILEDControl(b.GetReducedFromTop(130).GetFromTop(14).GetReducedFromRight(20).GetFromRight(20), COLOR_RED), kCtrlTagClipping);
    pGraphics->AttachControl(new ITextControl(b.GetReducedFromTop(130).GetFromTop(14).GetReducedFromRight(45).GetFromRight(37), "Clipping", IText(14)));

    const IVStyle redKnobStyle
    {
      true, // Show label
      true, // Show value
      {
        DEFAULT_BGCOLOR, // Background
        IColor(255, 161, 20, 31), // Foreground
        IColor(255, 201, 20, 31), // Pressed
        COLOR_WHITE, // Frame
        DEFAULT_HLCOLOR, // Highlight
        DEFAULT_SHCOLOR, // Shadow
        DEFAULT_X1COLOR, // Extra 1
        DEFAULT_X2COLOR, // Extra 2
        DEFAULT_X3COLOR  // Extra 3
      }, // Colors
    };

    const IVStyle blueKnobStyle
    {
      true, // Show label
      true, // Show value
      {
        DEFAULT_BGCOLOR, // Background
        IColor(255, 30, 60, 120), // Foreground
        IColor(255, 30, 60, 160), // Pressed
        COLOR_WHITE, // Frame
        DEFAULT_HLCOLOR, // Highlight
        DEFAULT_SHCOLOR, // Shadow
        DEFAULT_X1COLOR, // Extra 1
        DEFAULT_X2COLOR, // Extra 2
        DEFAULT_X3COLOR  // Extra 3
      }, // Colors
    };

    const IVStyle blackKnobStyle
    {
      true, // Show label
      true, // Show value
      {
        DEFAULT_BGCOLOR, // Background
        COLOR_BLACK, // Foreground
        COLOR_DARK_GRAY, // Pressed
        COLOR_WHITE, // Frame
        DEFAULT_HLCOLOR, // Highlight
        DEFAULT_SHCOLOR, // Shadow
        DEFAULT_X1COLOR, // Extra 1
        DEFAULT_X2COLOR, // Extra 2
        DEFAULT_X3COLOR  // Extra 3
      }, // Colors
    };

    // Knobs
    pGraphics->AttachControl(new IVKnobControl(b.GetReducedFromTop(170).GetFromTop(100).GetFromLeft(PLUG_WIDTH / 2).GetReducedFromLeft(PLUG_WIDTH / 6).GetCentredInside(100), kBits, "", redKnobStyle));
    pGraphics->AttachControl(new IVKnobControl(b.GetReducedFromTop(170).GetFromTop(100).GetFromRight(PLUG_WIDTH / 2).GetReducedFromRight(PLUG_WIDTH / 6).GetCentredInside(100), kSampleRate, "", blueKnobStyle));
    pGraphics->AttachControl(new IVKnobControl(b.GetReducedFromTop(300).GetFromTop(100).GetFromLeft(PLUG_WIDTH / 3).GetCentredInside(100), kInputGain, "", blackKnobStyle));
    pGraphics->AttachControl(new IVKnobControl(b.GetReducedFromTop(300).GetFromTop(100).GetReducedFromLeft(PLUG_WIDTH / 3).GetReducedFromRight(PLUG_WIDTH / 3).GetCentredInside(100), kMix, "", blackKnobStyle));
    pGraphics->AttachControl(new IVKnobControl(b.GetReducedFromTop(300).GetFromTop(100).GetFromRight(PLUG_WIDTH / 3).GetCentredInside(100), kOutputGain, "", blackKnobStyle));

    // Footnote thing
    pGraphics->AttachControl(new ITextToggleControl(b.GetReducedFromTop(430).GetFromTop(20).GetFromLeft(PLUG_WIDTH / 3).GetCentredInside(PLUG_WIDTH / 4), kClippingEnabled, "Clipping disabled", "Clipping enabled"));
    pGraphics->AttachControl(new ITextControl(b.GetReducedFromTop(430).GetFromTop(20).GetCentredInside(PLUG_WIDTH / 2), "Copyright 2021 Hannes Braun", IText(14)));
    pGraphics->AttachControl(new ITextControl(b.GetReducedFromTop(430).GetFromTop(20).GetFromRight(PLUG_WIDTH / 3), "Version 1.0.0", IText(14)));
  };
}

void BitReKt::OnIdle()
{
  // Controlling status LEDs
  if (!GetBypassed())
    SendControlValueFromDelegate(kCtrlTagSignalIn, 1.0);
  else
    SendControlValueFromDelegate(kCtrlTagSignalIn, 0.0);

  if (GetRenderingOffline())
    SendControlValueFromDelegate(kCtrlTagOfflineRender, 1.0);
  else
    SendControlValueFromDelegate(kCtrlTagOfflineRender, 0.0);

  if (clippingSignal.load())
    SendControlValueFromDelegate(kCtrlTagClipping, 1.0);
  else
    SendControlValueFromDelegate(kCtrlTagClipping, 0.0);
}

void BitReKt::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{
  const int nChans = NOutChansConnected();

  double bits = GetParam(kBits)->Value();
  double targetSampleRate = GetParam(kSampleRate)->Value();
  double inputGain = GetParam(kInputGain)->Value() / 100.0;
  double mix = GetParam(kMix)->Value() / 100.0;
  double outputGain = GetParam(kOutputGain)->Value() / 100.0;
  bool clipping = GetParam(kClippingEnabled)->Value();

  double drySignal;
  double tmpSignal;

  clippingSignal.store(false);
  bitCrusher.setBits(bits);
  downsampler.configure(targetSampleRate, GetSampleRate(), GetSamplePos());

  for (int s = 0; s < nFrames; s++)
  {
    for (int c = 0; c < nChans; c++)
    {
      drySignal = inputs[c][s] * inputGain;

      // Clipping
      if (clipping && drySignal > 1.0)
      {
        tmpSignal = 1.0;
        clippingSignal.store(true);
      }
      else if (clipping && drySignal < -1.0)
      {
        tmpSignal = -1.0;
        clippingSignal.store(true);
      }
      else
        tmpSignal = drySignal;

      tmpSignal = bitCrusher.crush(tmpSignal, 0.0);
      tmpSignal = downsampler.resample(tmpSignal, c);

      tmpSignal = tmpSignal * mix + drySignal * (1.0 - mix);
      outputs[c][s] = tmpSignal * outputGain;
    }
    downsampler.nextSample();
  }
}
