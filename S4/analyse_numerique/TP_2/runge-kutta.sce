clear
close

function [t,y] = RK4(t0, y0, T, N, f)
    // t0 et y0 les conditions initiales
    // T le temps
    // N le nombre max d'iteration
    // f une fonction
    h = T/N
    t = [t0]
    y = [y0]
    for n=1:N
        k1 = h*f(t(n), y(n))
        k2 = h*f(t(n) + h/2, y(n) + k1/2)
        k3 = h*f(t(n) + h/2, y(n) + k2/2)
        k4 = h*f(t(n) + h, y(n) + k3)
        
        y(n+1) = y(n) + (1/6)*(k1 + 2*k2 + 2*k3 + k4)
        t(n+1) = t(n) + h
    end
endfunction

function [t,y] = RK2(t0, y0, T, N, f)
    // t0 et y0 les conditions initiales
    // T le temps
    // N le nombre max d'iteration
    // f une fonction
    h = T/N
    t = [t0]
    y = [y0]
    for n=1:N
        k1 = h*f(t(n), y(n))
        k2 = h*f(t(n) + h, y(n) + k1)

        y(n+1) = y(n) + (1/2) * (k1 + k2)
        t(n+1) = t(n) + h
    end
endfunction

function y = test(t,x)
    y = -x+t+1
endfunction

T = 1
t0 = 0
y0 = 1


N=40
[t,y] = RK4(t0, y0, T, N, test)
plot(t,y,"g")

N=40
[t,y] = RK2(t0, y0, T, N, test)
plot(t,y,"r")

y = exp(-t)+t

plot(t,y,":b")
