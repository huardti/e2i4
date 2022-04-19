clear
close

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

/* Exo 2 */
/*

function y = test(t,x)
    y = -x+t+1
endfunction

T = 1
t0 = 0
y0 = 1

N=10
[t,y] = euler_explicite(t0, y0, T, N, test)
plot(t,y,"b")

N=20
[t,y] = euler_explicite(t0, y0, T, N, test)
plot(t,y,"r")

N=40
[t,y] = euler_explicite(t0, y0, T, N, test)
plot(t,y,"g")

y = exp(-t)+t

plot(t,y,":b")
*/

/* Exo 3 */
/*
function y = test(t,x)
    y = -150*x + 30
endfunction

T = 1
t0 = 0
N=50

y0 = 1/5
[t,y] = euler_explicite(t0, y0, T, N, test)
plot(t,y,"b")

e = 0.0000000001
y0 = 1/5+e
[t,y] = euler_explicite(t0, y0, T, N, test)
plot(t,y,":r")

*/
