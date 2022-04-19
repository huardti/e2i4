clear
close

function y = test(t, x)
    y = -2*x
endfunction

function y = test1(t, x)
    y = -10*x
endfunction

function y = test3(t, x)
    y = -150*x
endfunction


function [t,y] = euler_explicite(t0, y0, N, h, f)
    // t0 et y0 les conditions initiales
    // N le nombre max d'iteration
    // h le pas de discretisation
    // f une fonction
    t = [t0]
    y = [y0]
    for n=1:N
        y(n+1) = y(n) + h*f(t(n),y(n))
        t(n+1) = t(n) + h
    end
endfunction
/*
t0 = 0
y0 = 1
T = 10

hbarre = 2/abs(-2)

h = hbarre
N = 10/h
[t,y] = euler_explicite(t0, y0, N,  h, test)
plot(t,y,"b")

h = hbarre/4
N = 10/h
[t,y] = euler_explicite(t0, y0, N,  h, test)
plot(t,y,"g")

h = hbarre/10
N = 10/h
[t,y] = euler_explicite(t0, y0, N,  h, test)
plot(t,y,"r")

y = exp(-2*t)
plot(t,y,":b")
legend("h=hbarrre", "h=hbarre/4", "h=hbarrre/10", "solution analytique")
*/

/*
t0 = 0
y0 = 1
T = 1000

alpha = -2
hbarre = 2/abs(alpha)
h = hbarre/10
N = T/h
[t,y] = euler_explicite(t0, y0, N,  h, test)
plot(t,y,"r")

alpha = -10
hbarre = 2/abs(alpha)
h = hbarre/10
N = T/h
[t,y] = euler_explicite(t0, y0, N,  h, test1)
plot(t,y,"g")

alpha = -150
hbarre = 2/abs(alpha)
h = hbarre/10
N = T/h
[t,y] = euler_explicite(t0, y0, N,  h, test2)
plot(t,y,"b")
*/