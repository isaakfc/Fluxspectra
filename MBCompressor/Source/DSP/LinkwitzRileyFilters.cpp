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


//******************* LOWPASS 4th order ******************* ///

LinkwitzRiley4thOrderLowPass::LinkwitzRiley4thOrderLowPass(float cutoff, int sampleRate) : mSampleRate(sampleRate)
{
    updateCoefs(cutoff);
}




void LinkwitzRiley4thOrderLowPass::updateCoefs(float cutoff)
{
    double wc      = 2* M_PI * cutoff;
    double wc2     = wc * wc;
    double wc3     = wc2 * wc;
    double wc4     = wc2 * wc2;
    double k       = wc / tan(M_PI * cutoff/mSampleRate);
    double k2      = k * k;
    double k3      = k2 * k;
    double k4      = k2 * k2;
    double sqrt2   = sqrt(2);
    double sq_tmp1 = sqrt2 * wc3 * k;
    double sq_tmp2 = sqrt2 * wc * k3;
    double a_tmp   = 4 * wc2 * k2 + 2 * sq_tmp1 + k4 + 2 * sq_tmp2 + wc4;

    b1 = (4*(wc4 + sq_tmp1 - k4 - sq_tmp2)) / a_tmp;
    b2 = (6*wc4 - 8 * wc2 * k2 + 6 * k4) / a_tmp;
    b3 = (4*(wc4 - sq_tmp1 + sq_tmp2 - k4)) / a_tmp;
    b4 = (k4 - 2 * sq_tmp1 + wc4 - 2 * sq_tmp2 + 4 * wc2 * k2) / a_tmp;
    
    a0 = wc4 / a_tmp;
    a1 = 4 * wc4 / a_tmp;
    a2 = 6 * wc4 / a_tmp;
    a3 = a1;
    a4 = a0;

}

float LinkwitzRiley4thOrderLowPass::process(float input)
{
    
    float tempx = input;

    float tempy = a0 * tempx + a1*xm1 + a2*xm2 + a3*xm3 + a4*xm4 - b1*ym1 - b2*ym2 - b3*ym3 - b4*ym4;
    xm4 = xm3;
    xm3 = xm2;
    xm2 = xm1;
    xm1 = tempx;
    ym4 = ym3;
    ym3 = ym2;
    ym2 = ym1;
    ym1 = tempy;

    return tempy;
}

//******************* HIGHPASS 4th order ******************* ///

LinkwitzRiley4thOrderHighPass::LinkwitzRiley4thOrderHighPass(float cutoff, int sampleRate) : mSampleRate(sampleRate)
{
    updateCoefs(cutoff);
}

void LinkwitzRiley4thOrderHighPass::updateCoefs(float cutoff)
{
    double wc      = 2* M_PI * cutoff;
    double wc2     = wc * wc;
    double wc3     = wc2 * wc;
    double wc4     = wc2 * wc2;
    double k       = wc / tan(M_PI * cutoff/mSampleRate);
    double k2      = k * k;
    double k3      = k2 * k;
    double k4      = k2 * k2;
    double sqrt2   = sqrt(2);
    double sq_tmp1 = sqrt2 * wc3 * k;
    double sq_tmp2 = sqrt2 * wc * k3;
    double a_tmp   = 4 * wc2 * k2 + 2 * sq_tmp1 + k4 + 2 * sq_tmp2 + wc4;

    b1 = (4*(wc4 + sq_tmp1 - k4 - sq_tmp2)) / a_tmp;
    b2 = (6*wc4 - 8 * wc2 * k2 + 6 * k4) / a_tmp;
    b3 = (4*(wc4 - sq_tmp1 + sq_tmp2 - k4)) / a_tmp;
    b4 = (k4 - 2 * sq_tmp1 + wc4 - 2 * sq_tmp2 + 4 * wc2 * k2) / a_tmp;
    
    a0 = k4 / a_tmp;
    a1 = -4*k4 / a_tmp;
    a2 = 6*k4 / a_tmp;
    a3 = a1;
    a4 = a0;

}

float LinkwitzRiley4thOrderHighPass::process(float input)
{
    
    float tempx = input;

    float tempy = a0 * tempx + a1*xm1 + a2*xm2 + a3*xm3 + a4*xm4 - b1*ym1 - b2*ym2 - b3*ym3 - b4*ym4;
    xm4 = xm3;
    xm3 = xm2;
    xm2 = xm1;
    xm1 = tempx;
    ym4 = ym3;
    ym3 = ym2;
    ym2 = ym1;
    ym1 = tempy;

    return tempy;
}
