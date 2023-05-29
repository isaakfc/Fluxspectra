/*
  ==============================================================================

    RingBuffer.h
    Created: 22 May 2023 8:57:31pm
    Author:  Isaac Faraz-Chandler

  ==============================================================================
*/

#pragma once
#include <vector>


class RingBuffer{
public:
    RingBuffer(int size);
    double read(int delay);
    void write(double val);
    void reset(int size, int pos);
    double readInterp(double delay);
    double readInterp3(double delay);
private:
    std::vector<double> mBuffer;
    int mPos;
};
