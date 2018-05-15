%% synthesis

% core parameters
a = 3; % schlalfi numerator
b = 1; % schalfi denominator (a > 2b)
n = a/b; % order (schlalfi symbol)
f0 = 440; % f0
T = 0.0; % teeth
phaseOffset = 0; % initial phase
R = 1; % scale

fs = 44100;
dPhase = 2*pi * (f0/fs); % phase increment
sizeP = ceil(fs/f0); % period in samples

theta = phaseOffset; % init phase radial amplitude calculation
p = zeros(1, fs); % radial amplitude of geometry
for i=1:fs % geometry
    p(i) = cos(pi/n) / cos(mod(theta, 2*pi/n) -pi/n + T) * R;
    theta = theta+dPhase;
end

theta = 0; % init phase for sampling
poly = zeros(1, fs); % sampled geometry
for i=1:fs
    poly(i) = p(i) * (cos(theta) + 1j*sin(theta));
    theta = theta+dPhase;
end

waveform = imag(poly); % projection to y axis

%% anti aliasing

dx = diff(waveform); % first derivative of the waveform
dx = dx / max(abs(dx)); % normalisation
dx = [dx dx(1)]; % wrap around

waveformAA = waveform; % anti-aliased waveform
disc = zeros(1, a); % location of discontinuities expressed in samples

for k=1:a % iterate through the discontinuities in the first period
    
    disc(k) = fs/(n*f0)*k - fs/f0/(2*pi/phaseOffset) + 1;
   
    % boundary sample (wrapped around a single period)
    n3 = ceil(disc(k));
    n1 = n3-2;
    n2 = n1+1;
    n4 = n3+1;
    
    d = n3 - disc(k); % fractional delay between the discontinuity and the next sample 
    
    u = -2*tan(pi/n) * cos((2*pi/n)*k); % slope of the derivative at the discontinuity
   
    % 4-point polyBLAMP residual coefficients
    p0 = d^5/120;
    p1 = (-3*d^5 +5*d^4 +10*d^3 +10*d^2 +5*d +1)/120;
    p2 = (3*d^5 -10*d^4 +40*d^2 -60*d +28)/120;
    p3 = (-d^5 +5*d^4 -10*d^3 +10*d^2 -5*d +1)/120;
    
    % waveform correction on the four samples around the discontinuity
    waveformAA(n1) = waveform(n1) -p0*u *sign(waveform(n1));
    waveformAA(n2) = waveform(n2) -p1*u *sign(waveform(n2));
    waveformAA(n3) = waveform(n3) -p2*u *sign(waveform(n3)); 
    waveformAA(n4) = waveform(n4) -p3*u *sign(waveform(n4));
end

%% plot

subplot(2,1,1);
plot(real(poly), imag(poly), 'r');
axis([-1 1 -1 1]);
axis equal;
title('Complex plane');

subplot(2,1,2);
graph1 = plot(waveform, 'b');
axis([1 sizeP -1 1]);
hold on;
graph2 = plot(dx, '--');
hold on;
graph3 = plot(waveformAA, '-.m');
for i=1:a
    line([disc(i),disc(i)], [-1,1],'Color','red','LineStyle',':');
end
legend([graph1, graph2, graph3],{'Waveform', 'Derivative', 'Anti-aliased waveform'});
title('Projection');

%% output

duration = 1;

y = zeros(1, duration*fs);
readIndex = 1;

for i=1:length(y)
    y(i) = waveform(readIndex);
    
    readIndex = mod(readIndex+1, sizeP);
    if(readIndex == 0)
        readIndex = 1;
    end
end

soundsc(y, fs);