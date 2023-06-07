/*
  ==============================================================================

    DynamicsEngine.h
    Created: 24 May 2023 4:27:40pm
    Author:  Isaac Faraz-Chandler

  ==============================================================================
*/

#pragma once

//enum CompressionMode
//{
//    Compressor,
//    Limiter
//};
//enum DetectionType
//{
//    Peak,
//    RMS
//};

class DynamicsEngine
{
public:
    DynamicsEngine(int sampleRate);
    void setParameters(bool compOn, bool feedback, bool peakOn,float threshold, float attack, float release, float knee, float ratio);
    float process(float input, float sideChain);
    float processPeakCompressor(float input, float sideChain);
    float processRMSCompressor(float input, float sideChain);
    float processPeakLimiter(float input, float sideChain);
    float processRMSLimiter(float input, float sideChain);
    void updateCompStaticCharacteristics(float inputDb);
    void updateLimStaticCharacteristics(float inputDb);
    void gainSmoothPeak(float gainChangeDb);
    void gainSmoothRMS(float gainChangeDb);
protected:
    int mSampleRate;
    bool mfeedback;
//    CompressionMode mCompMode;
//    DetectionType mDetection;
    bool mCompressionOn;
    bool mPeakOn;
    float mT;
    float mA;
    float mR;
    float mW;
    float mRa;
    float alphaA1;
    float alphaR1;
    float mGainSc;
    float mgainSmoothPrev;
    float mYPrev;
    float mGainSmooth;
};




