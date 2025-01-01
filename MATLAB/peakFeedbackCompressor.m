function [y, lin_A] = peakFeedbackCompressor(x,T,R,W,alphaA,alphaR,N,LD)

% Initialize linear gain
lin_A = zeros(N,1);

% Initialise delay buffer
buffer = zeros(44100,1);

% Initialize smoothing variable
gainSmoothPrev = 0;

y_prev = 0; % Initialize output for feed-back detection

% Loop over each sample to see if it is above thresh
for n = 1:N
    % Turn the input signal into a uni-polar signal on the dB scale
    y_uni = abs(y_prev);
    y_dB = 20*log10(y_uni/1);
    % Ensure there are no values of negative infinity
    if y_dB < -96
        y_dB = -96;
    end
    
    % Static Characteristics
    if y_dB > (T + W/2)
        gainSC = T + (y_dB - T)/R; % Perform Downwards Compression for above knee curve
    elseif y_dB > (T - W/2)
        gainSC = y_dB + ((1/R - 1)*(y_dB - T + W/2)^2)/(2*W);
    else
        gainSC = y_dB; % Do not perform compression 
    end
          
    gainChange_dB = gainSC - y_dB;
    
    % smooth over the gainChange
    if gainChange_dB < gainSmoothPrev
        % attack mode
        gainSmooth = ((1-alphaA)*gainChange_dB) ...
            +(alphaA*gainSmoothPrev);
    else
        % release
        gainSmooth = ((1-alphaR)*gainChange_dB) ...
            +(alphaR*gainSmoothPrev);
    end

    % Convert to linear amplitude scalar
    lin_A(n,1) = 10^(gainSmooth/20);

    % Get delayed input
    [delayedInput,buffer] = fixedDelay(x(n,1),buffer,n,LD);
    
    % Apply linear amplitude to input sample
    y(n,1) = lin_A(n,1) * delayedInput;

    % Update for next cycle
    y_prev = y(n,1); 
    
    % Update gainSmoothPrev used in the next sample of the loop
    gainSmoothPrev = gainSmooth;
end