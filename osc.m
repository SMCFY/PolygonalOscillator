fs = 44100;
dur = 1;
t = [0:1/fs:dur-(1/fs)];

% core parameters
f = 80; theta = 2*pi*f*t; % phase angle
n = 4; % order (vertex count)
T = 0; % teeth
phaseOffset = pi/4; % initial phase

for i=1:length(theta)
%r(i) = cos(pi/n) / cos(2*pi/n * mod(theta(i)*n/2*pi, 1) - pi/n + T); % polygon amplitude
r(i) = cos(pi/n) ./ cos(mod(theta(i), 2*pi/n) -pi/n + T);
end

%poly = ones(1,fs+1) .* (cos(theta+phaseOffset) + 1j*sin(theta+phaseOffset)); % sinewave
poly = r .* (cos(theta+phaseOffset) + 1j*sin(theta+phaseOffset)); % sampled polygon
polyWav = imag(poly); % projection to y axis

%% plot

subplot(2,1,1);
plot(real(poly), imag(poly));

subplot(2,1,2);
plot(polyWav);

%% sound
soundsc(polyWav, fs);