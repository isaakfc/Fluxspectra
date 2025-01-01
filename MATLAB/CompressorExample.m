

[x,Fs] = audioread('piano_melody.wav');


y = CompressorPrototype(x,Fs,-30,4,0.1,0.2,2,-40,4,0.1,0.2,2, ...
    500,1,6,'rms-compressor',10);

%y = CompressorPrototype(x,Fs,-12,4,0.1,1,2,-5,4,0.01,0.06,2, ...
    %500,1,6,'peak-compressor',10);


sound(x,Fs);



%[x,Fs] = audioread('song.wav');

%y = CompressorPrototype(x,Fs,-30,10,0.1,1,4,-40,18,0.05,0.15,4, ...
    %1000,1,3,'peak-limiter',100);


x = x(:,1);

audiowrite("piano_melody_input.wav", x, Fs);

%audiowrite("song_Ouput.wav", y, Fs);