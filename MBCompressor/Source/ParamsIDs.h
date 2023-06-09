/*
  ==============================================================================

    ParamsIDs.h
    Created: 11 May 2023 9:07:10am
    Author:  Isaac Faraz-Chandler

  ==============================================================================
*/

#pragma once

namespace ParamIDs
{

    inline constexpr auto attackLb   { "Attack Time Low Band" };
    inline constexpr auto attackHb   { "Attack Time High Band" };
    inline constexpr auto releaseLb   { "Release Time Low Band" };
    inline constexpr auto releaseHb   { "Release Time High Band" };
    inline constexpr auto thresholdLb  { "Threshold Level Low Band"  };
    inline constexpr auto thresholdHb  { "Threshold Level High Band"  };
    inline constexpr auto kneeLb  { "Knee Low Band"  };
    inline constexpr auto kneeHb  { "Knee High Band"  };
    inline constexpr auto ratioLb { "Ratio Low Band" };
    inline constexpr auto ratioHb { "Ratio High Band" };
    inline constexpr auto gain { "Input Gain" };
    inline constexpr auto gainHb { "High band Gain" };
    inline constexpr auto gainLb { "Low band Gain" };
    inline constexpr auto makeup { "Makeup Gain" };
    inline constexpr auto mode { "Compression On" };
    inline constexpr auto source { "Sidechain Input" };
    inline constexpr auto crossover { "Crossover Point" };
    inline constexpr auto delay { "Lookahead-Delay" };
    inline constexpr auto delayButton { "Lookahead-Delay on/off" };
    inline constexpr auto feedback { "Feedback on/off" };
    inline constexpr auto mix { "Mix" };
    inline constexpr auto detection { "Peak on" };
    inline constexpr auto detectionRMS { "RMS on" };
    inline constexpr auto limitOn { "Limiting on" };
}
