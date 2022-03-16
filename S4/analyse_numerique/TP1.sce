
x = [0 0.1 0.2 0.5 0.8 0.9 1]
y = [30 29 24 19 18 15 0]
/*
X = 0:0.05:1
Y = Newton(x,y,X)

plot(x,y,"red")
plot(X,Y,"*")

xlabel("Proportion de l année")
ylabel("Puissance(GW)")
*/
/*
Y=interpln([x;y],X);

plot(x,y, "red")
plot(X,Y, "*")

xlabel("Proportion de l année")
ylabel("Puissance(GW)")

*/
XX = linspace(0,1,400)

d = splin(x,y)
yyi = interp(XX, x, y, d);

plot(x,y, "red")
plot(XX,yyi, "blue")
xlabel("Proportion de l année")
ylabel("Puissance(GW)")

inttrap(XX,yyi)
