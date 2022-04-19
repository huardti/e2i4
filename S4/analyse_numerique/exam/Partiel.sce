//Exo1
clear
close

function Y = Lagrange(x, y, X)
    n = length(x)
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


x=[0,10,20,30,40,50,60,70,80,90,100];
y=[100,84.7135,76.2285,49.5312,33.5646,20.9830,12.9488,10,12.0038,18.2018,27.3379];
X=0:1:100 ;
Y = Lagrange(x, y, X)
[YY,YYp]=interp(X,x,y,splin(x,y,'natural'))
plot(x,y,"*");
plot(X,YY,'red');
plot(X,Y,'blue');
xlabel("Distance parcourue sur le tremplin (m)");
ylabel("Hauteur (m)");
legend("Points mesurés","interp splin cubique","lagrange");


LF = sqrt(1+YYp .* YYp)
L=inttrap(X, YYp)

