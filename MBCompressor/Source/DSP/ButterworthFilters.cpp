/*
  ==============================================================================

    ButterworthFilters.cpp
    Created: 29 May 2023 1:31:56am
    Author:  Isaac Faraz-Chandler

  ==============================================================================
*/

#include "ButterworthFilters.h"
#include <cmath>

//******************* LOWPASS ******************* ///

ButterworthLowPass::ButterworthLowPass(float cutoff, int sampleRate) : x1(0.0), x2(0.0),y1(0.0),y2(0.0),mSampleRate(sampleRate)
{
    updateCoefs(cutoff);
}

void ButterworthLowPass::updateCoefs(float cutoff)
{
    double C = 1 / tan((M_PI * cutoff) / mSampleRate);

    mA0 = 1 / (1 + (sqrt(2) * C) + (C*C));
    mA1 =  2 * mA0;
    mA2 = mA0;
    mB1 = 2 * mA0 * ( 1 - (C * C));
    mB2 = mA0 * ( 1 - (sqrt(2) * C) + (C * C));
    mC0 = 1;
    mD0 = 0;
     
}

float ButterworthLowPass::process(float input)
{
    
    float x = input;
    float y = mD0 * x + mC0*(mA0 * x + mA1 * x1 + mA2 * x2 - mB1 * y1 - mB2 * y2);
    x2 = x1;
    x1 = x;
    y2 = y1;
    y1 = y;
    
    return y;
}


//******************* HIGHPASS ******************* ///

ButterworthHighPass::ButterworthHighPass(float cutoff, int sampleRate) : x1(0.0), x2(0.0),y1(0.0),y2(0.0),mSampleRate(sampleRate)
{
    updateCoefs(cutoff);
}

void ButterworthHighPass::updateCoefs(float cutoff)
{
    double C = tan((M_PI * cutoff) / mSampleRate);

    mA0 = 1 / (1 + (sqrt(2) * C) + (C*C));
    mA1 =  -2 * mA0;
    mA2 = mA0;
    mB1 = 2 * mA0 * ( (C * C) - 1);
    mB2 = mA0 * ( 1 - (sqrt(2) * C) + (C * C));
    mC0 = 1;
    mD0 = 0;
     
}

float ButterworthHighPass::process(float input)
{
    
    float x = input;
    float y = mD0 * x + mC0*(mA0 * x + mA1 * x1 + mA2 * x2 - mB1 * y1 - mB2 * y2);
    x2 = x1;
    x1 = x;
    y2 = y1;
    y1 = y;
    
    return y;
}
