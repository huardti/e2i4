Ts = 1;
%Zetas = [0.1, 0.5, 0.8];
Zetas = [0.1];
omega0 = 0.05

for Zeta = Zetas
    Gp = tf(omega0*omega0, [1, 2*Zeta*omega0, omega0*omega0]);
    
    Gzi = c2d(Gp, Ts, 'impulse');
    Gzz = c2d(Gp, Ts, 'zoh');
    
    fprintf("Zeta=%d\n",Zeta);
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