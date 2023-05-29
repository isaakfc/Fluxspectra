/*
  ==============================================================================

    LinkwitzRileyFilters.h
    Created: 23 May 2023 6:27:05pm
    Author:  Isaac Faraz-Chandler

  ==============================================================================
*/

#pragma once

class LinkwitzRiley2ndOrderLowPass
{
public:
    LinkwitzRiley2ndOrderLowPass(float cutoff, int sampleRate);
    void updateCoefs(float cutoff);
    float process(float input);
protected:
    double mB1, mB2;
    double mA0, mA1, mA2;
    double x1, x2;
    double y1, y2;
    int mSampleRate;
    double mC0 = 1;
    double mD0 = 0;
};

class LinkwitzRiley2ndOrderHighPass
{
public:
    LinkwitzRiley2ndOrderHighPass(float cutoff, int sampleRate);
    void updateCoefs(float cutoff);
    float process(float input);
protected:
    double mB1, mB2;
    double mA0, mA1, mA2;
    double x1, x2;
    double y1, y2;
    int mSampleRate;
    double mC0 = 1;
    double mD0 = 0;
};


class LinkwitzRiley4thOrderLowPass
{
public:
    LinkwitzRiley4thOrderLowPass(float cutoff, int sampleRate);
    void updateCoefs(float cutoff);
    float process(float input);
protected:
    double a0, a1, a2, a3, a4;
    double b1, b2, b3, b4;
    double xm1, xm2, xm3, xm4;
    double ym1, ym2, ym3, ym4;
    int mSampleRate;
};

class LinkwitzRiley4thOrderHighPass
{
public:
    LinkwitzRiley4thOrderHighPass(float cutoff, int sampleRate);
    void updateCoefs(float cutoff);
    float process(float input);
protected:
    double a0, a1, a2, a3, a4;
    double b1, b2, b3, b4;
    double xm1, xm2, xm3, xm4;
    double ym1, ym2, ym3, ym4;
    int mSampleRate;
};
