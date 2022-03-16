function F = fx(x)
    F =  ( (1 ./ sqrt(2 * %pi) * exp( - (1./2) * x*x) ))
endfunction

function X = compute_xk(a,k,h)
    X = a + k*h
endfunction

function I = trapeze(a, b, n, F)
    h = (b - a) / n
    s = 0
    for i =0:n-1
        s = s + (F(compute_xk(a,i+1,h)) + F(compute_xk(a,i,h)))/2
    end

    I = h * s
endfunction
