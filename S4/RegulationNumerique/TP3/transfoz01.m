Tarray = [0.4343, 0.6213, 0.8306, 1.4427, 1.9576, 2.8037, 4.4814, 9.4912];
Larray = cell(size(Tarray));
Te = 1;


figure()
i = 1
for T = Tarray
    Larray{i} = "T = " + T
    
    Gp = tf(1,[T, 1]);
    Gz = c2d(Gp, Te, 'zoh');
    
    fprintf("T=%d\n",T);
    Gp
    Gz
    
    hold on
    impulse(Gz)
    hold off
    
    i = i+1;
end
legend(Larray)

figure()
for T = Tarray    
    Gp = tf(1,[T, 1]);
    Gz = c2d(Gp, Te, 'zoh');
    
    hold on
    step(Gz)
    hold off    
end
legend(Larray)

figure()
for T = Tarray    
    Gp = tf(1,[T, 1]);
    Gz = c2d(Gp, Te, 'zoh');
    
    hold on
    pzmap(Gz)
    hold off    
end
legend(Larray)

figure()
for T = Tarray    
    Gp = tf(1,[T, 1]);
    Gz = c2d(Gp, Te, 'zoh');
    
    hold on
    bode(Gz)
    hold off    
end
legend(Larray)