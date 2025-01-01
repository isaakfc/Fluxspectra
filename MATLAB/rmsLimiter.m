function [y, lin_A] = rmsLimiter(x,T,W,alphaA,alphaR,N,LD)

% Initialize linear gain
lin_A = zeros(N,1);

% Initialise delay buffer
buffer = zeros(44100,1);

% Initialize smoothing variable
gainSmoothPrev = 0;

% Loop over each sample to see if it is above thresh
for n = 1:N
    
    % Turn the input signal into a uni-polar signal on the dB scale
    x_dB = 20*log10(abs(x(n,1)));
    
    % Ensure there are no values of negative infinity
    if x_dB < -96
        x_dB = -96;
    end
    
     % Static Characteristics
    if x_dB > (T + W/2)
        gainSC = T; 
    elseif x_dB > (T - W/2)
        gainSC = x_dB - (x_dB - T + W/2)^2/(2*W);
    else
        gainSC = x_dB; 
    end
          
    gainChange_dB = gainSC - x_dB;
    
    % smooth over the gainChange
    if gainChange_dB < gainSmoothPrev
        % attack mode
        gainSmooth = -sqrt(((1-alphaA)*gainChange_dB^2) ...
            +(alphaA*gainSmoothPrev^2));
    else
        % release
        gainSmooth = -sqrt(((1-alphaR)*gainChange_dB^2) ...
            +(alphaR*gainSmoothPrev^2));
    end

    % Convert to linear amplitude scalar
    lin_A(n,1) = 10^(gainSmooth/20);

    % Get delayed input
    [delayedInput,buffer] = fixedDelay(x(n,1),buffer,n,LD);
    
    % Apply linear amplitude to input sample
    y(n,1) = lin_A(n,1) * delayedInput;
    
    % Update gainSmoothPrev used in the next sample of the loop
    gainSmoothPrev = gainSmooth;
   
end