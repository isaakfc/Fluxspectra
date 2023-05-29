/*
  ==============================================================================

    ButterworthFilters.h
    Created: 29 May 2023 1:31:56am
    Author:  Isaac Faraz-Chandler

  ==============================================================================
*/

#pragma once

class ButterworthLowPass
{
public:
    ButterworthLowPass(float cutoff, int sampleRate);
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

class ButterworthHighPass
{
public:
    ButterworthHighPass(float cutoff, int sampleRate);
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
