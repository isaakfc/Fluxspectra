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



