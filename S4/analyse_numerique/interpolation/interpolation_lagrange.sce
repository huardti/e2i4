function Y = Lagrange(x, y, X)
    n = size(x)(2)
    Y = 0
    for i=1:n,
        L = 1
        for j=1:n,
            if j ~= i then
                L = L .* (X-x(j)) / (x(i) - x(j))
            end
        end      
        Y = Y + y(i) * L 
    end
endfunction

function P = Newton(x, y, X)
    n = size(x)(2) - 1
    
    for k=1:n+1,
        a(k) = y(k)
    end

    for s=1:n,
        for k=n+1:-1:s+1,
            a(k) = (a(k) - a(k - 1)) / (x(k) - x(k - s))
        end
    end
    
    P = a(n+1)
    for i=n+1:-1:2,
        P = P.*(X-x(i-1)) + a(i-1)
    end
endfunction

function[P]=newton(X,Y)//X nodes,Y values;P is the numerical Newton polynomial
n=length(X);// n is the number of nodes. (n-1) is the degree
for j=2:n,
  for i=1:n-j+1,Y(i,j)=(Y(i+1,j-1)-Y(i,j-1))/(X(i+j-1)-X(i));end,
end,
x=poly(0,"x");
P=Y(1,n);
for i=2:n, P=P*(x-X(i))+Y(i,n-i+1); end
endfunction;
