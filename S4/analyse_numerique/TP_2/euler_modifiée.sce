function [t,y] = euler_modifiee(t0, y0, T, N, f)
    // t0 et y0 les conditions initiales
    // T le temps
    // N le nombre max d'iteration
    // f une fonction
    h = T/N
    t = [t0]
    y = [y0]
    for n=1:N
        ny = y(n) + (h/2) * f(t(n), y(n))

        y(n+1) = y(n) + h*f(t(n) + h/2, ny)
        t(n+1) = t(n) + h
    end
endfunction

function [t,y] = euler_explicite(t0, y0, T, N, f)
    // t0 et y0 les conditions initiales
    // T le temps
    // N le nombre max d'iteration
    // f une fonction
    h = T/N
    t = [t0]
    y = [y0]
    for n=1:N
        y(n+1) = y(n) + h*f(t(n),y(n))
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
[t,y] = euler_modifiee(t0, y0, T, N, test)
plot(t,y,"g")

[t,y] = euler_explicite(t0, y0, T, N, test)
plot(t,y,"r")

y = exp(-t)+t

plot(t,y,":b")
