clear
close 
clf

function y=f(x)
    y= x^2-2;
endfunction

function y=F(x)
    y= 2*x;
endfunction

function ret = newton(f, F, x0, err)
    /* f la fonction, F sa derivée, a le point de depart, err l'erreur accepter */

    epsilon       = 1e-7;
    maxIterations = 20;

    for i = 1:maxIterations
      y = f(x0);
      yprime = F(x0);
    
      if abs(yprime) < epsilon            // Stop if the denominator is too small
        ret = null;
      end;
    
      x1 = x0 - y/yprime;                 // Do Newton's computation
    
      if abs(x1 - x0) <= err              // Stop when the result is within the desired tolerance
        ret = x1;
      end;
    
      x0 = x1;                            // Update x0 to start the process again
    end;
endfunction

function [y, xx, k]=newton2(f,fdiv,esp,x0)
        /* f la fonction, F sa derivée, a le point de depart, err l'erreur accepter */
    xk = x0
    xk1 = 0
    k = 0
    xx = [xk]
    while abs(f(xk)) > esp
        xk1 = xk - f(xk)/ F(x0);                 // Do Newton's computation
        xk = xk1;                            // Update x0 to start the process again
        k = k+1
        xx(k) = xk
    end;
    y = xk;
endfunction


plot2d(log(abs(xx(1:k-1)-sqrt(2)), log(abs(xx(2:k)-sqrt(2)))
