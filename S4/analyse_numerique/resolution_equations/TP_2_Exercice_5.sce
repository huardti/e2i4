//Exercice 5

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

t = [0 3 4]
u = [-8 4 0]

X = 0:0.1:4
Y=Lagrange(t,U,X)

plot(X,Y)

plot(t,U, "r")
