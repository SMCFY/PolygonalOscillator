%% synthesis

% core parameters
a = 4; % schlalfi numerator
b = 1; % schalfi denominator (a > 2b)
n = a/b; % order (schlalfi symbol)
f0 = 440; % f0
T = 0.0; % teeth
phaseOffset = pi/4; % initial phase
R = 1; % scale

fs = 44100;
dPhase = 2*pi*f0*(1/fs); % phase increment
sizeP = ceil(fs/f0*b); % period in samples

f0_lowest = 20; % lowest possible fundamental frequency
waveSize = fs/f0_lowest; % size of the generated waveform in samples

theta = phaseOffset; % init phase with offset, for radial amplitude calculation
p = zeros(1, waveSize); % radial amplitude of geometry
for i=1:waveSize % geometry
    %p(i) = cos(pi/n) / cos(mod(theta, 2*pi/n) -pi/n + T) * R;
    p(i) = cos(pi/n) / cos(2*pi/n * mod(theta*n/(2*pi), 1) -pi/n + T) * R;
    theta = theta+dPhase;
end

theta = 0; % init phase for sampling
poly = zeros(1, waveSize); % sampled geometry
for i=1:waveSize
    poly(i) = p(i) * (cos(theta) + 1j*sin(theta));
    theta = theta+dPhase;
end

waveform = imag(poly); % projection to y axis

%% anti aliasing

dx = diff(waveform,1); % first derivative of the waveform
%dx = dx / max(abs(dx)); % normalisation

discSlope = diff(waveform,2); % second derivative
discSlope = [0, discSlope]; % offset
%discSlope = discSlope / abs(max(discSlope));

waveformAA = waveform; % anti-aliased waveform
disc = zeros(1, a); % location of discontinuities expressed in samples
discPhase = zeros(1, a); % location of discontinuities expressed in radians

u = zeros(1, a); % change in amplitude at discontinuities

for k=1:a % iterate through the discontinuities in the first period
    
    disc(k) = fs/(n*f0)*k - fs/f0/(2*pi/phaseOffset)+1;
    discPhase(k) = 2*pi/n*k;
   
    % boundary samples
    n3 = ceil(disc(k));
    n1 = n3-2;
    n2 = n1+1;
    n4 = n3+1;
    
    d = n3 - disc(k); % fractional delay between the discontinuity and the next sample 
    
    %u(k) = abs(-2*tan(pi/n) * cos(dPhase*disc(k))+phaseOffset); % slope of the derivative at the discontinuity
    %u(k) = abs(-2*tan(pi/n)*cos(discPhase(k)));
    %u(k) = abs(discSlope(n2));
    u(k) = abs(discSlope(n2)+ (disc(k)-n2) * ((discSlope(n3)-discSlope(n2)) / (n3-n2)) ) *2;
    
    % 4-point polyBLAMP residual coefficients
    p0 = d^5/120;
    p1 = (-3*d^5 +5*d^4 +10*d^3 +10*d^2 +5*d +1)/120;
    p2 = (3*d^5 -10*d^4 +40*d^2 -60*d +28)/120;
    p3 = (-d^5 +5*d^4 -10*d^3 +10*d^2 -5*d +1)/120;
    
    % waveform correction on the four samples around the discontinuity
    waveformAA(n1) = waveform(n1) -p0*u(k) *sign(waveform(n1));
    waveformAA(n2) = waveform(n2) -p1*u(k) *sign(waveform(n2));
    waveformAA(n3) = waveform(n3) -p2*u(k) *sign(waveform(n3)); 
    waveformAA(n4) = waveform(n4) -p3*u(k) *sign(waveform(n4));
end

%% plot

subplot(2,2,1);
plot(real(poly), imag(poly), 'r');
axis([-1 1 -1 1]);
axis equal;
title('Complex plane');

subplot(2,2,3);
graph1 = plot(waveform, 'b');
axis([1 sizeP -1 1]); % display the first period only
hold on;
graph2 = plot(dx, '--');
hold on;
graph3 = plot(waveformAA, '-.m');
hold on;
graph4 = plot(discSlope, 'g');
for i=1:a
    line([disc(i),disc(i)], [-1,1],'Color','red','LineStyle',':');
end
legend([graph1, graph2, graph3, graph4],{'Original waveform', 'Derivative', 'Anti-aliased waveform', 'Second derivative'});
title('Projection');

% magnitude spectrum comparison
subplot(2,2,2);
plot(db(abs(fft(waveform, fs))), 'k');
axis([0 fs/2 -40 60]);
title('Original waveform');

subplot(2,2,4);
plot(db(abs(fft(waveformAA, fs))), 'k');
axis([0 fs/2 -40 60]);
title('Anti-aliased waveform');

%% SNR

magSpec = abs(fft(waveform));
%magSpec = magSpec - max(magSpec); % offset
dFreq = length(magSpec)/fs; % frequency resolution

eSig = 0; % energy of the marmonics
fH = [f0, zeros(1, 20)]; % frequency of the first k harmonics and the fundamental (Hz)
for k=2:length(fH) % sum of the energy of the first k harmonics
    fH(k) = f0*(2*floor(k/2)+1+(n-2)*(1+floor((k-1)/2))); 
    
    eSig = eSig + ( magSpec(f0*dFreq+1) + magSpec(fH(k)*dFreq+1) )^2;
end 

magEn = 0; % summed energy over the whole spectrum
for i=1:length(magSpec)
    magEn = magEn + magSpec(i)^2;
end

eNoise = magEn - eSig; % energy of the noise

snr = eSig / eNoise;
disp('SNR: ');disp(snr);

%% output

duration = 1;

y = zeros(1, duration*fs);
readIndex = 1;

for i=1:length(y)
    y(i) = waveformAA(readIndex);
    
    readIndex = mod(readIndex+1, sizeP);
    if(readIndex == 0)
        readIndex = 1;
    end
end

%soundsc(y, fs);