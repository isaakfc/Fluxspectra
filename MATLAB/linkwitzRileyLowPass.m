% LINKWITZ-RILEY

function [out] = linkwitzRileyLowPass(in,Fs,f0)

%%% Initial Parameters
N = length(in);
out = zeros(length(in),1);

%%% Intermediate Variables

theta = pi*f0/Fs;           
omega = pi*f0;      
kappa  = omega / tan(theta); 
delta = kappa^2 + omega^2 + (2 * kappa * omega);

a0 = omega^2 / delta;
a1 =  2 * (omega^2)/delta;
a2 = (omega^2)/delta;
b1 = (-2*kappa^2 + 2*omega^2) / delta;
b2 = (-2*kappa*omega + kappa^2 + omega^2) / delta;
c0 = 1;
d0 = 0;

% Initial conditions 
x2 = 0;   
x1 = 0;
y2 = 0;
y1 = 0;

for n = 1:N
    out(n,1) = d0*in(n,1) + c0*(a0*in(n,1) + a1*x1 + a2*x2 - b1*y1 - b2*y2);
    x2 = x1;
    x1 = in(n,1);
    y2 = y1;
    y1 = out(n,1);
end


end


