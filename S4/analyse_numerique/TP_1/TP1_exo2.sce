
x = [1965 1970 1980 1985 1990 1991]
y = [17768 24001 25961 34336 29036 33417]
X = 1961:0.1:1993

Y=interpln([x;y],X);

plot(x,y, "red")
plot(X,Y, "blue")


xlabel("Année")
ylabel("Production(x10⁵Kg)")
legend("points mesurés", "interpolation Newton", "interpolation splin")
inttrap(XX,yyi)

