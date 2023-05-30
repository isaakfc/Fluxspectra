/*
  ==============================================================================

    Utils.h
    Created: 22 May 2023 1:20:34am
    Author:  Isaac Faraz-Chandler

  ==============================================================================
*/

#pragma once
#include <cmath>

float equalPowerMix(float input, float output, float mix)
{
    mix = mix / 100;
    return sqrt((1.0 - mix)/2) * input + sqrt(mix/2) * output;
}



