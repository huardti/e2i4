Tarray = [ 0.4343 0.6213 0.8306 ; 2.8037 4.4814 9.4912 ]
Larray = cell(size(Tarray)-1);
Te = 1;


figure()
i = 1
for T = Tarray
     Larray{i} = "T1 = " + T(1) + " T2 = " + T(2)

    Gp = tf(1, [T(1)*T(2), T(1)+T(2), 1]);
    Gz = c2d(Gp, Te, 'zoh');
    
    fprintf("T1=%d, T2=%d\n",T(1), T(2));
    Gp
    Gz
    
    hold on
    step(Gz)
    hold off
    
    i = i+1
end
legend(Larray)

figure()
for T = Tarray
    Gp = tf(1, [T(1)*T(2), T(1)+T(2), 1]);
    Gz = c2d(Gp, Te, 'zoh');
    
    hold on
    impulse(Gz)
    hold off
end
legend(Larray)

figure()
for T = Tarray
    Gp = tf(1, [T(1)*T(2), T(1)+T(2), 1]);
    Gz = c2d(Gp, Te, 'zoh');
    
    hold on
    pzmap(Gz)
    hold off
end
legend(Larray)

figure()
for T = Tarray
    Gp = tf(1, [T(1)*T(2), T(1)+T(2), 1]);
    Gz = c2d(Gp, Te, 'zoh');
    
    hold on
    bode(Gz)
    hold off
end
legend(Larray)