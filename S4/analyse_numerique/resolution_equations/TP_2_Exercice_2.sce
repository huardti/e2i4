function y = f(x)
    y = abs(x^2 - 0.25)
endfunction

function I = simpson(a,b,F)
    I = ((b-a)/6) * (F(a) + 4 * F((a+b)/2) + F(b))
endfunction

function I = simpson_aprox(a,b,n,F)
    val = a:((b-a)/n):b
    I = 0
    for i = 1: (n-1)
        I = I + simpson(val(i), val(i+1), F)
    end
endfunction
