function y=f(x)
    y=-3*x+4;
endfunction

function ret = dichitomie(a, b, err, F)
    /* a et b les bornes , err l'erreur et F la fonction */
    while(b-a > err)
        m = (a + b) / 2;
        if(F(m) * F(a) > 0) then a = m
        else b = m;
        end
    end
    ret = m;
endfunction
