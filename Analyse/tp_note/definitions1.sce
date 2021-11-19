delta = (2*%pi)/200;
t = 0:delta:2*%pi;
s2 = sin(t) - (1/2)*sin(2*t);
plot(t, s2);
