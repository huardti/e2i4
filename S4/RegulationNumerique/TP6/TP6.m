close all
clear all

Te = 0.4;

Gp = tf(1, [10, 1]);
Gz = c2d(Gp, Te, 'zoh');

Td = tf(1,[4,1]);
Tdz = c2d(Td, Te, 'zoh');


A = Gz.Denominator{1,1};
B = Gz.Numerator{1,1};
P = Tdz.Denominator{1,1};

r0 = A(2)/B(2)
r1 = (P(2)-A(2)+1)/B(2)

Cz = tf([r1, r0], [1, -1], Te);

L = series(Cz, Gz);
%L=Cz*Gz,
T = feedback(L, 1)


F = tf([r0+r1], [r1 r0], Te);
FTBFfiltree=F*T;

figure()
subplot(2,2,1)
bode(Gz)
legend()
subplot(2,2,2)
bode(Cz)
legend()
subplot(2,2,3)
bode(L)
legend()
subplot(2,2,4)
bode(T)
legend()

figure()
nyquist(L)
axis([-2, 0, -8, 8])
legend()

figure()
step(T,Tdz)

legend()

figure()
step(FTBFfiltree,Tdz)
legend()

stepinfo(T, 'SettlingTimeThreshold', 0.01)
stepinfo(Tdz, 'SettlingTimeThreshold', 0.01)
stepinfo(FTBFfiltree, 'SettlingTimeThreshold', 0.01)
