function y=f(x)
    y= x - exp(1) * sin(x);
endfunction

function y=F(x)
    y= exp(1) * cos(x);
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
