Te = 0.03
K = 2.5
Gp = tf(1,[1, 1, 0]);
Gz = c2d(Gp, Te, 'zoh');

L = series(K, Gz)
T = feedback(L, 1)

figure()
subplot(1, 2, 1)
bode(L)
subplot(1, 2, 2)
nyquist(L)
sgtitle("Boucle ouverte L(z)")

figure()
subplot(1, 2, 1)
bode(T)
subplot(1, 2, 2)
pzmap(T)
sgtitle("Boucle fermée T(z)")

figure()
hold on
impulse(T)
impulse(Gp)
hold off
legend("Boucle fermée T(z)", "Procédé Gp(z)")

[yi, t] = impulse(T);
yi(1:5)

figure()
hold on
step(T)
step(Gp)
hold off
legend("Boucle fermée T(z)", "Procédé Gp(z)")

[ys, t] = step(T);
ys(1:5)