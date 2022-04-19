clear
close

function [t,y]=euler_implicite(t0, y0, T, N, f)
    h = T/N
    t = [t0]
    y = [y0]
    for n=1:N
        t(n+1) = t(n)+h;
        function ret = g(in)
            ret = y(n) + h*f(t(n+1), in)-in 
        endfunction
        y(n+1)= fsolve(y(n), g)
    end;
endfunction

function y = test(t,x)
    y = -150*x + 30
endfunction

T = 1
t0 = 0
N=50

y0 = 1/5
[t,y] = euler_implicite(t0, y0, T, N, test)
plot(t,y,"g")

e = 10e-10
y0 = 1/5 + e
[t,y] = euler_implicite(t0, y0, T, N, test)
plot(t,y,"r")
