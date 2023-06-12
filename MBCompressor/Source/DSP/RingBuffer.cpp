/*
  ==============================================================================

    RingBuffer.cpp
    Created: 22 May 2023 8:57:31pm
    Author:  Isaac Faraz-Chandler

  ==============================================================================
*/

#include "RingBuffer.h"
#include <iostream>
#include <tgmath.h>

RingBuffer::RingBuffer(int size)
{
    mBuffer = std::vector<double>(size, 0);
    mPos = 0;
}

double RingBuffer::read(int delay)
{
    delay = std::min(delay, (int)(mBuffer.size() - 1));
    int index = mPos - delay;
    if(index < 0)index += mBuffer.size();
    return mBuffer[index];
}

double RingBuffer::readInterp(double delay)
{
    int samples = int(delay);
    double v1 = read(samples);
    double v2 = read(samples + 1);
    double frac = delay - samples;
    return v1 + frac * (v2 - v1);
}

void RingBuffer::write(double val)
{
    mBuffer[mPos] = val;
    mPos = (mPos + 1) % mBuffer.size();
}

void RingBuffer::reset(int size, int pos)
{
    mBuffer = std::vector<double>(size, 0);
    mPos = pos;
}


