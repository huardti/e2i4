T = 10;
%Te = [0.01*T, 0.1*T, 0.5*T];
%Te = [1];
Te = [0.01*T]
    
%Larray = [0.5, 2, 3];
Larray = [2];

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
        
        figure()
        impulse(Gzz)
        figure()
        step(Gzz)
        figure()
        pzmap(Gzz)
        figure()
        bode(Gzz)
    end
end