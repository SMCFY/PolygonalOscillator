% wavetable
tableSize = 1024;
%waveTable = zeros(1, tableSize);

% core parameters
np = 2; % number of periods
n = 5/np; % order (schlalfi symbol) (n>2)
f = 440/np; % f0
T = 0.0; % teeth
phaseOffset = 0; % initial phase
R = 0.8; % scale


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
%waveTable = [waveTable, waveTable(1)]; % wrap waveform around

%% plot
subplot(2,1,1);
plot(real(poly), imag(poly), 'r');
axis([-1 1 -1 1]);
axis equal;

subplot(2,1,2);
plot(waveTable, 'r');
axis([0 tableSize -1 1]);

%% sound
duration = 1; 
fs = 44100;
tableOverSamplingRatio = tableSize / fs;
tableDelta = f * tableOverSamplingRatio; % read increment for wavetable
readIndex = 1; % table read index

y = zeros(1, fs*duration); % output


for n=1:length(y) % synhtesis from wavetable
    
    % boundary indices and corresponding values
    i1 = floor(readIndex+1); 
    if i1 == tableSize
        i2 = 1;
    else
        i2 = i1+1;
    end
    v1 = waveTable(i1);
    v2 = waveTable(i2);
  
    frac = readIndex - i1; % sample fraction
    
    y(n) = v2 + (frac*(v2-v1)); %interpolation
    
    
    readIndex = readIndex + tableDelta;
    if readIndex > tableSize
        readIndex = readIndex-tableSize;
    end
    
end



soundsc(y, fs);
%figure();
%plot(y);