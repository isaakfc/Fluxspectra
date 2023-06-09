/*
  ==============================================================================

    DynamicsEngine.cpp
    Created: 24 May 2023 4:27:40pm
    Author:  Isaac Faraz-Chandler

  ==============================================================================
*/

#include "DynamicsEngine.h"
#include <cmath>
#include <iostream>
#include <algorithm>

DynamicsEngine::DynamicsEngine(int sampleRate)
{
    mSampleRate = sampleRate;
    mYPrev = 0;
}

void DynamicsEngine::setParameters(bool compOn, bool feedback, bool peakOn,float threshold, float attack, float release, float knee, float ratio)
{
    
    mCompressionOn = compOn;
    mPeakOn        = peakOn;
    mfeedback      = feedback;
    mA             = attack;
    mR             = release;
    mW             = knee;
    mRa            = ratio;
    mT             = threshold;
    alphaA1        = std::exp(-std::log(9)/(mSampleRate * attack));
    alphaR1        = std::exp(-std::log(9)/(mSampleRate * release));
    mGainSmooth    = 0;
    
}

float DynamicsEngine::process(float input, float sideChain)
{
    float output = 0;
    
//    switch (mCompressionOn)
//    {
//        case true:
//        {
//            if (mPeakOn == true)
//            {
//                output = processPeakCompressor(input, sideChain);
//            }
//            else
//            {
//                output = processRMSCompressor(input, sideChain);
//            }
//        }
//            break;
//        case false:
//        {
//            if (mPeakOn == true)
//            {
//                output = processPeakLimiter(input, sideChain);
//            }
//            else
//            {
//                output = processRMSLimiter(input, sideChain);
//            }
//        }
//            break;
//    }
    
    if (mCompressionOn)
    {
        if (mPeakOn)
        {
//            std::cout << "PEAK COMPRESSION" << std::endl;
            output = processPeakCompressor(input, sideChain);
        }
        else
        {
//            std::cout << "RMS COMPRESSION" << std::endl;
            output = processRMSCompressor(input, sideChain);
        }
    }
    else
    {
        if (mPeakOn)
        {
//            std::cout << "PEAK LIMITING" << std::endl;
            output = processPeakLimiter(input, sideChain);
        }
        else
        {
//            std::cout << "RMS LIMITING" << std::endl;
            output = processRMSLimiter(input, sideChain);
        }
    }

    return output;
}

float DynamicsEngine::processPeakCompressor(float input, float sideChain)
{
    
    float xUni = 0;
    
    if(mfeedback)
    {
        xUni = std::abs(mYPrev);
    }
    else
    {
        xUni = std::abs(sideChain);
    }
        
    float xDb = -96;
    if (xUni > 0)
    {
        xDb = 20 * log10(xUni);
    }
    
    updateCompStaticCharacteristics(xDb);
    
    
    float gainChangeDb = mGainSc - xDb;
    
    gainSmoothPeak(gainChangeDb);
    
    float outputGain = pow(10, mGainSmooth / 20);
    float output = outputGain * input;
    
    mYPrev = output;
    
    mgainSmoothPrev = mGainSmooth;
    
    return output;
    
}

float DynamicsEngine::processRMSCompressor(float input, float sideChain)
{
    
    float xUni = 0;
    
    if(mfeedback)
    {
        xUni = std::abs(mYPrev);
    }
    else
    {
        xUni = std::abs(sideChain);
    }
        
    float xDb = -96;
    
    if (xUni > 0)
    {
        xDb = 20 * log10(xUni);
    }
    
    updateCompStaticCharacteristics(xDb);
    
    float gainChangeDb = mGainSc - xDb;
    
    gainSmoothRMS(gainChangeDb);
    
    float outputGain = pow(10, mGainSmooth / 20);
    float output = outputGain * input;
    
    mYPrev = output;
    
    mgainSmoothPrev = mGainSmooth;
    
    return output;
    
}


float DynamicsEngine::processPeakLimiter(float input, float sideChain)
{
    
    float xUni = std::abs(sideChain);
    
    float xDb = -96;
    
    if (xUni > 0)
    {
        xDb = 20 * log10(xUni);
    }
    
    updateLimStaticCharacteristics(xDb);
    
    float gainChangeDb = mGainSc - xDb;
    
    gainSmoothPeak(gainChangeDb);
    
    float outputGain = pow(10, mGainSmooth / 20);
    float output = outputGain * input;
    
    mgainSmoothPrev = mGainSmooth;
    
    return output;
    
}

float DynamicsEngine::processRMSLimiter(float input, float sideChain)
{
    
    float xUni = std::abs(sideChain);
        
    float xDb = -96;
    
    if (xUni > 0)
    {
        xDb = 20 * log10(xUni);
    }
    
    updateLimStaticCharacteristics(xDb);
    
    float gainChangeDb = mGainSc - xDb;
    
    gainSmoothRMS(gainChangeDb);
    
    float outputGain = pow(10, mGainSmooth / 20);
    float output = outputGain * input;
    
    mgainSmoothPrev = mGainSmooth;
    
    return output;
}






void DynamicsEngine::updateCompStaticCharacteristics(float inputDb)
{
    if (inputDb > (mT + mW/2))
    {
        mGainSc = mT + (inputDb - mT)/mRa;
    }
    else if (inputDb > (mT - mW/2))
    {
        mGainSc = inputDb + ((1/mRa - 1) * (inputDb - mT + mW/2) * (inputDb - mT + mW/2))  / (2 * mW);
    }
    else
    {
        mGainSc = inputDb;
    }
}


void DynamicsEngine::updateLimStaticCharacteristics(float inputDb)
{
    if (inputDb > (mT + mW/2))
    {
        mGainSc = mT;
    }
    else if (inputDb > (mT - mW/2))
    {
        mGainSc = inputDb - ((inputDb - mT + mW/2) * (inputDb - mT +mW/2)) / (2*mW);
    }
    else
    {
        mGainSc = inputDb;
    }
}


void DynamicsEngine::gainSmoothPeak(float gainChangeDb)
{
    if (gainChangeDb < mgainSmoothPrev)
    {
        mGainSmooth = ( (1-alphaA1) * gainChangeDb ) +( alphaA1 * mgainSmoothPrev);
    }
    else
    {
        mGainSmooth = ( (1-alphaR1) * gainChangeDb) + (alphaR1 * mgainSmoothPrev);
    }
}




void DynamicsEngine::gainSmoothRMS(float gainChangeDb)
{
    if (gainChangeDb < mgainSmoothPrev)
    {
        mGainSmooth = -sqrt(std::max((1 - alphaA1) * (gainChangeDb * gainChangeDb) + (alphaA1 * (mgainSmoothPrev * mgainSmoothPrev)), 0.0f));
    }
    else
    {
        mGainSmooth = -sqrt(std::max((1 - alphaR1) * (gainChangeDb * gainChangeDb) + (alphaR1 * (mgainSmoothPrev * mgainSmoothPrev)), 0.0f));
    }
}


