x = [0 6 12 18 24 30 36 42 48 54 60 66 72 78 84]
y = [38 41 45.5 47.5 44.8 40.5 37 33 30 26 23.5 27 32 35 37.5]
plot(x,y,"red")

X = 0:1:84
Y = Newton(x,y,X)
plot(X,Y,"green")

// Y=interpln([x;y],X);
// plot(X,Y, "blue")

Y = splin(x,y)
YY = interp(X, x, y, Y);

plot(X,YY, "blue")


xlabel("temps (s)")
ylabel("vitesse (m/s)")
legend("points mesurés", "interpolation Newton", "interpolation splin")

inttrap(X,YY)
