# Fluxspectra

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Architecture](#architecture)
- [Implementation](#implementation)
  - [Look-ahead Delay](#look-ahead-delay)
  - [Band Splitting](#band-splitting)
  - [Dynamics Processing](#dynamics-processing)
- [Interface](#interface)
- [References](#references)

## Overview

Fluxspectra is a multiband dynamics processor implemented as a VST/AU plugin using the JUCE framework. It features dual-band compression/limiting with comprehensive visual feedback and modern DSP techniques. The plugin was developed as part of master's research into real-time audio processing and plugin development, with complete implementations in both MATLAB (prototype) and C++ (final plugin).

## Features
- Dual-band compression and limiting with peak/RMS detection
- Look-ahead delay up to 20ms with feedback detection option
- Real-time spectrum analyser and comprehensive metering
- Linkwitz-Riley crossover filters
- Automatic gain compensation

## Architecture

The plugin architecture centers around a multiband dynamics processing core where incoming audio is split into frequency bands for independent processing. As shown in the signal flow diagram, audio first passes through a look-ahead delay buffer before being split by Linkwitz-Riley filters into high and low frequency bands. Each band then feeds into its own dynamics engine for processing.

<br>
<img src="https://github.com/user-attachments/assets/d107ba80-723a-4be4-9433-6adabe75f15e" alt="Architecture Overview" width="550"/>
<br>
<br>

The dynamics engine employs a multi-stage gain calculation system for each band. The audio is converted to dB scale, processed through a compressor/limiter curve based on threshold, ratio and knee parameters, and smoothed using attack/release envelopes before being applied as linear gain:

<br>
<img src="https://github.com/user-attachments/assets/168d0bad-32d8-4f9d-9b88-5318c6823e83" alt="Gain Calculator" width="550"/>
<br>

## Implementation

### Look-ahead Delay

The look-ahead delay system uses a ring buffer implementation that provides sample-accurate delay between 1-20ms. The delay line uses linear interpolation for smooth time offset processing, with a boolean control for enabling/bypassing the delay.

```cpp
// CPP
class RingBuffer {
    double readInterp(double delay);
    void write(double val);
    void reset(int size, int pos);
};
```

```matlab
% MATLAB
function [out,buffer] = fixedDelay(in, buffer, n, delay)
```

### Band Splitting

The frequency band separation employs Linkwitz-Riley filters to ensure perfect magnitude reconstruction when the bands are recombined. Each filter includes coefficient calculation and per-sample processing:

```cpp
// CPP
class LinkwitzRiley2ndOrderLowPass {
    void updateCoefs(float cutoff);
    float process(float input);
};

class LinkwitzRiley2ndOrderHighPass {
    void updateCoefs(float cutoff);
    float process(float input);
};
```

```matlab
% MATLAB
function [out] = linkwitzRileyLowPass(in, Fs, f0)
function [out] = linkwitzRileyHighPass(in, Fs, f0)
```

### Dynamics Processing

The dynamics processing system provides both compression and limiting with multiple detection modes. The implementation allows for feedback/feedforward topology and includes sophisticated gain smoothing:

```cpp
// CPP
class DynamicsEngine {
    void setParameters(bool compOn, bool feedback, bool peakOn, 
                      float threshold, float attack, float release, 
                      float knee, float ratio);
    float processPeakCompressor(float input, float sideChain);
    float processRMSCompressor(float input, float sideChain);
};
```

```matlab
% MATLAB
function [y, lin_A] = peakCompressor(x, T, R, W, alphaA, alphaR, N, LD)
function [y, lin_A] = rmsCompressor(x, T, R, W, alphaA, alphaR, N, LD)
function [y, lin_A] = peakFeedbackCompressor(x, T, R, W, alphaA, alphaR, N, LD)
```

## Interface

The plugin features a modern, flat design interface with real-time visualisation and intuitive controls:

<img src="https://github.com/user-attachments/assets/3251b4de-143c-42f5-b3d6-23ff39d84232" alt="Interface" width="600" style="object-fit: cover; height: calc(100% - 50px);"/>  
<br>

**Key interface components**:
- Real-time spectrum analyser with frequency grid
- Per-band gain reduction metering
- Tabbed interface for band parameters
- Global controls for mode selection and crossover
- Input/output gain metering with multi-stage visualisation

## References

### Academic Sources
- Giannoulis, D., Massberg, M. and Reiss, J.D. (2012). 'Digital Dynamic Range Compressor Design'
- Pirkle, W. (2019). 'Designing Audio Effect Plugins in C++'
- Reiss, J.D. and McPherson, A. (2014) 'Audio Effects: Theory, Implementation and Application'
- Tarr, E. (2019) 'Hack Audio: An Introduction to Computer Programming and Digital Signal Processing in MATLAB'

### Code References & Tutorials
- MatKat Music (2022) 'PFM::SimpleMultiBandComp Full Tutorial'
- Murthy, A. (2022) 'Drawing Level Meters - Visualizations with JUCE'
- Viator, L. (2023) 'viatordsp'

## License

This project is provided for educational and research purposes. Code and design may be used with proper attribution.

