clear
close

function y = test(t, x)
    y = -2*x
endfunction

function [t,y]=euler_implicite(t0, y0, N, h, f)
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

t0 = 0
y0 = 1
T = 10

hbarre = 2/abs(-2)

h = hbarre
N = 10/h
[t,y] = euler_implicite(t0, y0, N,  h, test)
plot(t,y,"r")

hbarre = 2/abs(-2)

h = hbarre/10
N = 10/h
[t,y] = euler_implicite(t0, y0, N,  h, test)
plot(t,y,"b")

y = exp(-2*t)
plot(t,y,":b")
legend("h = hbarre", "h = hbarre/10", "solution analytique")

