
fs = 44100;
dur = 1;
t = [0:fs-1]/fs; 

% core parameters
f = 200; % f0
n = 4; % order (vertex count)
T = 0; % teeth
phaseOffset = pi/4; % initial phase

theta = 2*pi*f*t; % phase angle [radians]

% polygon 
r = zeros(1, length(theta)); % polygon amplitude
for i=1:length(r)
r(i) = cos(pi/n) / cos(mod(theta(i), 2*pi/n) -pi/n + T);
end


% sampling and projection
poly = r .* (cos(theta+phaseOffset) + 1j*sin(theta+phaseOffset)); % sampled polygon
polyWav = imag(poly); % projection to y axis

%% plot
subplot(2,1,1);
plot(real(poly), imag(poly));

subplot(2,1,2);
plot(polyWav);

%% sound
soundsc(polyWav, fs);