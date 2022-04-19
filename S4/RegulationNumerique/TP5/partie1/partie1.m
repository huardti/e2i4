K = 5.6;
a = 50;
b = 312;

Gp = tf(1740, [0.25, 1, 0]);
Carray = [1, 10, tf([5.6, 5.6*50], [1, 312])];


for C = Carray

    L = series(C, Gp);
    T = feedback(L, 1);

    figure()
    bode(L)
    sgtitle("Procédé Gp(s)")

    figure()
    subplot(1, 2, 1)
    bode(L)
    subplot(1, 2, 2)
    nyquist(L)
    axis([-1.5, -0.5, -1, 1])
    sgtitle("Boucle ouverte L(p)")

    figure()
    subplot(2, 2, 1)
    bode(T)
    subplot(2, 2, 2)
    pzmap(T)
    subplot(2, 2, 3)
    step(T)
    subplot(2, 2, 4)
    impulse(T)
    sgtitle("Boucle fermée T(p)")

    stepinfo(T, 'SettlingTimeThreshold', 0.01)
end