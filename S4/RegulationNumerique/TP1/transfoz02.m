T = 10;
Te = [0.01*T, 0.1*T, 0.5*T];

Larray = [0.5, 2, 3];

for Ts = Te
    for L = Larray    
        Gp = tf(1, [T, 1], 'InputDelay', L);
    
        Gzi = c2d(Gp, Ts, 'impulse');
        Gzz = c2d(Gp, Ts, 'zoh');
    
        fprintf("Ts=%d\n",Ts);
        fprintf("L=%d\n", L);
        Gp
        Gzi
        Gzz
    end
end