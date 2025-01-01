
function [out] = CompressorPrototype(in,Fs,T1,Ra1,A1,R1,K1,T2,Ra2,A2,R2,K2, ...
    crossoverFreq,inputGain,makeupGain,type,LD)

% Step Input Signal
stepIn = [zeros(Fs,1); ones(Fs,1); zeros(Fs,1)]; 
NStep = length(stepIn);
stepOutput = zeros(NStep,2);

% Convert to mono
in = in(:,1);

% Apply input gain
in = in * inputGain;

% Multiband processing
N = length(in);

inBands(:,1) = linkwitzRileyLowPass(in,Fs,crossoverFreq);
inBands(:,2) = linkwitzRileyHighPass(in,Fs,crossoverFreq);
inBands(:,2) = inBands(:,2) * -1;
bandOutput = zeros(N,2);

% Calculate lookahead delay in sample
% Convert to seconds
LD = LD/1000;
% Convert to samples
LD = LD * Fs;


% Initialize separate attack and release times
alphaA1 = exp(-log(9)/(Fs * A1));
alphaR1 = exp(-log(9)/(Fs * R1));
alphaA2 = exp(-log(9)/(Fs * A2));
alphaR2 = exp(-log(9)/(Fs * R2));


% COMPRESSORS

if strcmp(type,'peak-compressor')

% Compress with audio signal 
[bandOutput(:,1),lin_A1] = peakCompressor(inBands(:,1),T1,Ra1,K1,alphaA1,alphaR1,N,LD);
[bandOutput(:,2), lin_A2] = peakCompressor(inBands(:,2),T2,Ra2,K2,alphaA2,alphaR2,N,LD);

% Compress with step input
[stepOutput(:,1),lin_A1s] = peakCompressor(stepIn,T1,Ra1,K1,alphaA1,alphaR1,NStep,LD);
[stepOutput(:,2), lin_A2s] = peakCompressor(stepIn,T2,Ra2,K2,alphaA2,alphaR2,NStep,LD);

end

if strcmp(type,'peak-feedback-compressor')

% Compress with audio signal 
[bandOutput(:,1),lin_A1] = peakFeedbackCompressor(inBands(:,1),T1,Ra1,K1,alphaA1,alphaR1,N,LD);
[bandOutput(:,2), lin_A2] = peakFeedbackCompressor(inBands(:,2),T2,Ra2,K2,alphaA2,alphaR2,N,LD);

% Compress with step input
[stepOutput(:,1),lin_A1s] = peakFeedbackCompressor(stepIn,T1,Ra1,K1,alphaA1,alphaR1,NStep,LD);
[stepOutput(:,2), lin_A2s] = peakFeedbackCompressor(stepIn,T2,Ra2,K2,alphaA2,alphaR2,NStep,LD);

end

if strcmp(type,'rms-compressor')

% Compress with audio signal 
[bandOutput(:,1),lin_A1] = rmsCompressor(inBands(:,1),T1,Ra1,K1,alphaA1,alphaR1,N,LD);
[bandOutput(:,2), lin_A2] = rmsCompressor(inBands(:,2),T2,Ra2,K2,alphaA2,alphaR2,N,LD);

% Compress with step input
[stepOutput(:,1),lin_A1s] = rmsCompressor(stepIn,T1,Ra1,K1,alphaA1,alphaR1,NStep,LD);
[stepOutput(:,2), lin_A2s] = rmsCompressor(stepIn,T2,Ra2,K2,alphaA2,alphaR2,NStep,LD);


end

if strcmp(type,'rms-feedback-compressor')

% Compress with audio signal 
[bandOutput(:,1),lin_A1] = rmsFeedbackCompressor(inBands(:,1),T1,Ra1,K1,alphaA1,alphaR1,N,LD);
[bandOutput(:,2), lin_A2] = rmsFeedbackCompressor(inBands(:,2),T2,Ra2,K2,alphaA2,alphaR2,N,LD);

% Compress with step input
[stepOutput(:,1),lin_A1s] = rmsFeedbackCompressor(stepIn,T1,Ra1,K1,alphaA1,alphaR1,NStep,LD);
[stepOutput(:,2), lin_A2s] = rmsFeedbackCompressor(stepIn,T2,Ra2,K2,alphaA2,alphaR2,NStep,LD);


end


% LIMITERS

if strcmp(type,'peak-limiter')

% Limit with audio signal 
[bandOutput(:,1),lin_A1] = peakLimiter(inBands(:,1),T1,K1,alphaA1,alphaR1,N,LD);
[bandOutput(:,2), lin_A2] = peakLimiter(inBands(:,2),T2,K2,alphaA2,alphaR2,N,LD);

% Limit with step input
[stepOutput(:,1),lin_A1s] = peakLimiter(stepIn,T1,K1,alphaA1,alphaR1,NStep,LD);
[stepOutput(:,2), lin_A2s] = peakLimiter(stepIn,T2,K2,alphaA2,alphaR2,NStep,LD);


end

if strcmp(type,'rms-limiter')

% Limit with audio signal 
[bandOutput(:,1),lin_A1] = rmsLimiter(inBands(:,1),T1,K1,alphaA1,alphaR1,N,LD);
[bandOutput(:,2), lin_A2] = rmsLimiter(inBands(:,2),T2,K2,alphaA2,alphaR2,N,LD);

% Limit with step input
[stepOutput(:,1),lin_A1s] = rmsLimiter(stepIn,T1,K1,alphaA1,alphaR1,NStep,LD);
[stepOutput(:,2), lin_A2s] = rmsLimiter(stepIn,T2,K2,alphaA2,alphaR2,NStep,LD);


end


% Sum together all the bands
out = sum(bandOutput,2);
out = out * makeupGain;

sound(out, Fs);

CompressorPlot(bandOutput,stepOutput,Fs,N,NStep,in,stepIn,lin_A1,lin_A2,lin_A1s,lin_A2s,out);

end

