n = 100
a = 0
b = 1
x = a:1/(n-1):b
f = 1 ./ (1+x^2)
ref_val = integrate('1/(1+x^2)','x', a,b)

function F = test(x)
    F =  (1 ./ (1+x^2))
endfunction

function H = compute_h(a,b,n)
    H = (b - a) / n
endfunction

function X = compute_xk(a,k,h)
    X = a + k*h
endfunction

function I = rect_gauche(a, b, n, F)
    h = compute_h(a,b,n)
    s = 0
    for i = 0 : n-1
        s = s + F(compute_xk(a,i,h))
    end
    I = s * h
endfunction

function I = rect_milieu(a, b, n, F)
    h = compute_h(a,b,n)
    s = 0
    for i =0 : n-1
        s = s + F((compute_xk(a,i,h) + compute_xk(a,i+1,h)) /2)
    end
    I = h * s
endfunction

function I = trapeze(a, b, n, F)
    h = (b - a) / n
    s = 0
    for i =0:n-1
        s = s + (F(compute_xk(a,i+1,h)) + F(compute_xk(a,i,h)))/2
    end

    I = h * s
endfunction

function I = simpson(a,b,n,F)
    I = ((b-a)/6) * (F(a) + 4 * F((a+b)/2) + F(b))
endfunction

function R = calcul_erreur_rectangle_gauche(n, )
    R = 
endfunction
