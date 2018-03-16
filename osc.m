% core parameters
f = 120; % f0
n = 4; % order (vertex count)
T = 0; % teeth
phaseOffset = pi/4; % initial phase


% wavetable
tableSize = 256;
waveTable = zeros(1, tableSize+1);

t = [0:tableSize-1]/tableSize; % time vector
theta = 2*pi*1*t; % phase angles of a period
 
for i=1:tableSize % geometry
    waveTable(i) = cos(pi/n) / cos(mod(theta(i), 2*pi/n) -pi/n + T);
end

poly = zeros(1, tableSize); % sampled geometry
for i=1:tableSize
    poly(i) = waveTable(i) * (cos(theta(i)+phaseOffset) + 1j*sin(theta(i)+phaseOffset));
end

%poly = waveTable .* (cos(theta+phaseOffset) + 1j*sin(theta+phaseOffset)); % sampled polygon
waveTable = imag(poly); % projection to y axis
%waveTable = [waveTable, waveTable(1)]; % wrap waveform around

%% plot
subplot(2,1,1);
plot(real(poly), imag(poly));

subplot(2,1,2);
plot(waveTable);

%% sound
duration = 3; 
fs = 44100;
tableOverSamplingRate = tableSize / fs;
tableDelta = f * tableOverSamplingRate; % read increment for wavetable
readIndex = 1; % sample need to be read from wavetable

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
    
    y(n) = v1 + (frac*(v2-v1)); %interpolation
    
    
    readIndex = readIndex + tableDelta;
    if readIndex > tableSize
        readIndex = readIndex-tableSize;
    end
    
end



soundsc(y, fs);
figure();
plot(y);