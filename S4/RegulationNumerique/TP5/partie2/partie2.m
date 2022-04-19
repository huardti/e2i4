K = 5.6;
a = 50;
b = 312;
Te = 0.003;

Gp = tf(1740, [0.25, 1, 0]);
Cs = tf([K, K*a], [1, b]);

Cz = c2d(Cs, Te, 'matched')
Gz = c2d(Gp, Te, 'zoh')

Lz = series(Cz, Gz);
Tz = feedback(Lz, 1)

figure()
subplot(2, 2, 1)
bode(Tz)
subplot(2, 2, 2)
pzmap(Tz)
subplot(2, 2, 3)
step(Tz)
subplot(2, 2, 4)
impulse(Tz)
sgtitle("Boucle fermée T(z)")

stepinfo(Tz, 'SettlingTimeThreshold', 0.01)