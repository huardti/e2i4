function y = h(t)
    g = 9.806
    T = 0.4
    h0 = 90
    y = h0 - g*T*t + g*T^2 *(1 - exp(t/T))
endfunction

function ret = dichotomie(a, b, err, F)
    /* a et b les bornes , err l'erreur et F la fonction */
    while(b-a > err)
        m = (a + b) / 2;
        if(F(m) * F(a) > 0) then a = m
        else b = m;
        end
    end
    ret = m;
endfunction

t = dichotomie(0, 2, 0.0001, h)

/*
x = 0:0.01:1.8
y = h(x)
plot(x,y)
xtitle("""", "temps", "hauteur")
*/
