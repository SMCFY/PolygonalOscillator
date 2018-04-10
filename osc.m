%% wavetable

tableSize = 1024;
%waveTable = zeros(1, tableSize);

% core parameters
np = 1; % schalfi denominator (number of periods)
N = 5; % schlalfi numerator
n = N/np; % order (schlalfi symbol) (n>2)
f = 440/np; % f0
T = 0.0; % teeth
phaseOffset = 0; % initial phase
R = 1; % scale


t = [0:tableSize-1]/tableSize; % time vector
theta = 2*pi*t*np; % phase angle
p = zeros(1, tableSize); % radial amplitude of geometry
 
for i=1:tableSize % geometry
    p(i) = cos(pi/n) / cos(mod(theta(i), 2*pi/n) -pi/n + T) * R;
end

poly = zeros(1, tableSize); % sampled geometry
for i=1:tableSize
    poly(i) = p(i) * (cos(theta(i)+phaseOffset) + 1j*sin(theta(i)+phaseOffset));
end

waveTable = imag(poly); % projection to y axis

%% anti aliasing

dx = diff(waveTable)/0.1; % first derivative of the waveform
dx = dx / max(abs(dx)); % normalisation

waveTableAA = waveTable; % anti-aliased waveform

for i=1:N-1 % iterates through discontinuities
    nBoundary = ceil(tableSize/N*i); % boundary sample after the discontinuity
    d = nBoundary - (tableSize/N*i); % fractional delay between the discontinuity and the next sample
    u = abs(-2*tan(pi/n)*cos(2*pi/n*i)); % change in amplitude at the discontinuities
    
    % 4-point polyBLAMP residual coefficients
    p0 = d^5/120;
    p1 = (-3*d^5 +5*d^4 +10*d^3 +10*d^2 +5*d +1)/120;
    p2 = (3*d^5 -10*d^4 +40*d^2 -60*d +28)/120;
    p3 = (-d^5 +5*d^4 -10*d^3 +10*d^2 -5*d +1)/120;
    
    % waveform correction on the four samples around the discontinuity
    waveTableAA(nBoundary -2) = waveTable(nBoundary -2) -p0*sign(waveTable(nBoundary -2)) *u;
    waveTableAA(nBoundary -1) = waveTable(nBoundary -1) -p1*sign(waveTable(nBoundary -1)) *u;
    waveTableAA(nBoundary   ) = waveTable(nBoundary   ) -p2*sign(waveTable(nBoundary   )) *u; 
    waveTableAA(nBoundary +1) = waveTable(nBoundary +1) -p3*sign(waveTable(nBoundary +1)) *u;
    
end

%% plot

subplot(2,1,1);
plot(real(poly), imag(poly), 'r');
axis([-1 1 -1 1]);
axis equal;
title('Complex plane');

subplot(2,1,2);
plot(waveTable, 'b');
axis([0 tableSize -1 1]);
hold on;
plot(dx, '--');
hold on;
plot(waveTableAA, '-.m');
legend('Waveform', 'Derivative', 'Anti-aliased waveform');
title('Projection');
%% synthesis

duration = 1; 
fs = 44100;
tableOverSamplingRatio = tableSize / fs;
tableDelta = f * tableOverSamplingRatio; % read increment for wavetable
readIndex = 1; % table read index

y = zeros(1, fs*duration); % output


for i=1:length(y) % synhtesis from wavetable
    
    % boundary indices and corresponding values
    i1 = floor(readIndex+1); 
    if i1 == tableSize
        i2 = 1;
    else
        i2 = i1+1;
    end
    v1 = waveTableAA(i1);
    v2 = waveTableAA(i2);
  
    frac = readIndex - i1; % sample fraction
    
    y(i) = v2 + (frac*(v2-v1)); %interpolation
    
    
    readIndex = readIndex + tableDelta;
    if readIndex > tableSize
        readIndex = readIndex-tableSize;
    end
    
end



%soundsc(y, fs);
%figure();
%plot(y);