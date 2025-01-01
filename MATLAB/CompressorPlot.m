function CompressorPlot(bandOutput,stepOutput,Fs,N,NStep,in,stepIn,lin_A1,lin_A2,lin_A1s,lin_A2s,out)

%Plotting audio 
t = [0:N-1]/Fs; t = t(:);
subplot(4,1,1);
plot(t,in); title('Input Signal');axis([0 7 -1.1 1.1]); 
subplot(4,1,2);
plot(t,bandOutput(:,1),t,lin_A1); title('Band 1 Output'); axis([0 7 -1.1 1.1]);
legend('Band 1 Output','Gain Reduction');
subplot(4,1,3);
plot(t,bandOutput(:,2),t,lin_A2); title('Band 2 Output'); axis([0 7 -1.1 1.1]);
legend('Band 2 Output','Gain Reduction');
subplot(4,1,4);
plot(t,out); title('Output Signal');axis([0 7 -1.1 1.1]); 

%Plotting step input
figure();
Ts = 1/Fs;
tStep = [0:NStep-1]*Ts; tStep = tStep(:);
subplot(5,1,1);
plot(tStep,stepIn); title('Step Input');axis([0 3 -0.1 1.1]); 
subplot(5,1,2);
plot(tStep,stepOutput(:,1)); title('Comp Out Band 1 Settings'); axis([0 3 -0.1 1.1]);
subplot(5,1,3);
plot(tStep,lin_A1s); title('Gain Reduction Band 1 Settings');axis([0 3 -0.1 1.1]);
subplot(5,1,4);
plot(tStep,stepOutput(:,2)); title('Comp Out Band 2 Settings'); axis([0 3 -0.1 1.1]);
subplot(5,1,5);
plot(tStep,lin_A2s); title('Gain Reduction Band 2 Settings');axis([0 3 -0.1 1.1]);








end