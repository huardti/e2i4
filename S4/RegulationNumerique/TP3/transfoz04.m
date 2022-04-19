Zeta = 0.5;
OmegaArray = [ 0.6283, 1.2566, 1.8850, 2.5133, 3.1416 ];
Larray = cell(size(OmegaArray));

Te = 0.1;


figure()
i = 1
for omega = OmegaArray
    Larray{i} = "omega = " + omega;
    
    Gp = tf(omega*omega, [1, 2*Zeta*omega, omega*omega]);
    Gz = c2d(Gp, Te, 'zoh');
    
    fprintf("omega = %d\n", omega);
    Gp
    Gz
    
    hold on
    step(Gz)
    hold off
    
    i = i+1
end
legend(Larray)

figure()
for omega = OmegaArray
    Gp = tf(omega*omega, [1, 2*Zeta*omega, omega*omega]);
    Gz = c2d(Gp, Te, 'zoh');
    
    hold on
    impulse(Gz)
    hold off
end
legend(Larray)

figure()
for omega = OmegaArray
    Gp = tf(omega*omega, [1, 2*Zeta*omega, omega*omega]);
    Gz = c2d(Gp, Te, 'zoh');
    
    hold on
    pzmap(Gz)
    hold off
end
legend(Larray)

figure()
for omega = OmegaArray
    Gp = tf(omega*omega, [1, 2*Zeta*omega, omega*omega]);
    Gz = c2d(Gp, Te, 'zoh');
    
    hold on
    bode(Gz)
    hold off
end
legend(Larray)