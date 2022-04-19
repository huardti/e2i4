Ts = 1;
Zetas = [0.1, 0.5, 0.8];
omega0 = 0.05

for Zeta = Zetas
    Gp = tf(omega0*omega0, [1, 2*Zeta*omega0, omega0*omega0]);
    
    Gzi = c2d(Gp, Ts, 'impulse');
    Gzz = c2d(Gp, Ts, 'zoh');
    
    fprintf("Zeta=%d\n",Zeta);
    Gp
    Gzi
    Gzz
end