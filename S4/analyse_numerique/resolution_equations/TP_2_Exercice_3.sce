function y = dose(x)
    y = (exp(1)./3) .* x .* exp(-(x./3)) - 0.25
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

dichitomie(4, 24, 0.00001, dose)
