T = 10;
Te = [0.01*T, 0.1*T, 0.5*T];


for Ts = Te
    
    Gp = tf(1,[T, 1]);

    Gzi = c2d(Gp, Ts, 'impulse');
    Gzz = c2d(Gp, Ts, 'zoh');
    
    fprintf("Ts=%d\n",Ts);
    Gp
    Gzi
    Gzz
end