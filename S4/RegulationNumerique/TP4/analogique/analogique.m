Te = 1
K = 25
Gp = tf(1,[1, 1, 0]);

L = series(K, Gp)
T = feedback(L, 1)

figure()
subplot(1, 2, 1)
bode(L)
subplot(1, 2, 2)
nyquist(L)
sgtitle("Boucle ouverte L(s)")

figure()
subplot(1, 2, 1)
bode(T)
subplot(1, 2, 2)
pzmap(T)
sgtitle("Boucle ferm�e T(s)")

figure()
hold on
impulse(T)
impulse(Gp)
hold off
legend("Boucle ferm�e T(s)", "Proc�d� Gp(s)")

figure()
hold on
step(T)
step(Gp)
hold off
legend("Boucle ferm�e T(s)", "Proc�d� Gp(s)")