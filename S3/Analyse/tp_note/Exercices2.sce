function y = an_num(T, f, x, n)
retour = zeros(1, n)
for i=0:n-1,
    retour(1, i+1) = (2/T) * inttrap(x, f .* cos (2 * %pi * i * (1/T) .* x)),
end
y = retour
endfunction

function y = bn_num(T, f, x, n)
retour = zeros(1, n)
for i=1:n-1,
    retour(1, i+1) = (2/T) * inttrap(x, f .* sin (2 * %pi * i * (1/T) .* x)),
end
y = retour
endfunction

function y= fourier(T, f, x, t, N)
an = an_num(T,f,x,N)
bn = bn_num(T,f,x,N)

retour = 0
for i=1:N-1,
    retour = retour + ( an(1,i+1).*cos(2 * %pi * i * (1/T) .* t) + bn(1,i+1).*sin(2 * %pi * i * (1/T) .* t) ),
end
retour = retour + an(1,1)/2
y = retour
endfunction
