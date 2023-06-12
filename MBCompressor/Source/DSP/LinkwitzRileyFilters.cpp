/*
  ==============================================================================

    LinkwitzRileyFilters.cpp
    Created: 23 May 2023 6:27:05pm
    Author:  Isaac Faraz-Chandler

  ==============================================================================
*/

#include "LinkwitzRileyFilters.h"
#include <cmath>



//******************* LOWPASS 2nd order ******************* ///

LinkwitzRiley2ndOrderLowPass::LinkwitzRiley2ndOrderLowPass(float cutoff, int sampleRate)
{
    mSampleRate = sampleRate;
    updateCoefs(cutoff);
}

void LinkwitzRiley2ndOrderLowPass::updateCoefs(float cutoff)
{
    double theta = (M_PI * cutoff)/mSampleRate;
    double omega = M_PI * cutoff;
    double kappa  = omega / tan(theta);
    double delta = pow(kappa,2) + pow(omega,2) + (2 * kappa * omega);

    mA0 = pow(omega,2) / delta;
    mA1 =  2 * pow(omega,2)/delta;
    mA2 = pow(omega,2)/delta;
    mB1 = (-2 * pow(kappa,2) + 2*pow(omega,2)) / delta;
    mB2 = (-2*kappa*omega + pow(kappa,2)  + pow(omega,2)) / delta;
    mC0 = 1;
    mD0 = 0;
     
}

float LinkwitzRiley2ndOrderLowPass::process(float input)
{
    
    float x = input;
    float y = mD0 * x + mC0*(mA0 * x + mA1 * x1 + mA2 * x2 - mB1 * y1 - mB2 * y2);
    x2 = x1;
    x1 = x;
    y2 = y1;
    y1 = y;
    
    return y;
}

//******************* HIGHPASS 2nd order ******************* ///

LinkwitzRiley2ndOrderHighPass::LinkwitzRiley2ndOrderHighPass(float cutoff, int sampleRate)
{
    mSampleRate = sampleRate;
    updateCoefs(cutoff);
}

void LinkwitzRiley2ndOrderHighPass::updateCoefs(float cutoff)
{
    double theta = (M_PI * cutoff)/mSampleRate;
    double omega = M_PI * cutoff;
    double kappa  = omega / tan(theta);
    double delta = pow(kappa,2) + pow(omega,2) + (2 * kappa * omega);

    mA0 = pow(kappa,2) / delta;
    mA1 =  (-2 * pow(kappa,2))/delta;
    mA2 = pow(kappa,2)/delta;
    mB1 = (-2 * pow(kappa,2) + 2*pow(omega,2)) / delta;
    mB2 = (-2*kappa*omega + pow(kappa,2)  + pow(omega,2)) / delta;
    mC0 = 1;
    mD0 = 0;
     
}


float LinkwitzRiley2ndOrderHighPass::process(float input)
{
    
    float x = input;
    float y = mD0 * x + mC0*(mA0 * x + mA1 * x1 + mA2 * x2 - mB1 * y1 - mB2 * y2);
    x2 = x1;
    x1 = x;
    y2 = y1;
    y1 = y;
    
    return y;
}


