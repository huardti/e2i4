T1 = 1;
T2 = 10;
%Te = [0.01*T2, 0.1*T2, 0.5*T2];
%Te = [1];
Te = [0.01*T2]

for Ts = Te
    Gp = tf(1, [-T1*T2, T1-T2, 1]);
    
    Gzi = c2d(Gp, Ts, 'impulse');
    Gzz = c2d(Gp, Ts, 'zoh');
    
    fprintf("Ts=%d\n",Ts);
    Gp
    Gzi
    Gzz
    
    figure()
    impulse(Gzz)
    figure()
    step(Gzz)
    figure()
    pzmap(Gzz)
    figure()
    bode(Gzz)
end