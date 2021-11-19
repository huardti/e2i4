function y = somme(n,t)
    s=0
    for k=1:n,
        s = s + ((1/k) * sin(k * t)),
    end
    y = s
endfunction


tt = 0:delta:2*%pi;
plot(tt, somme(10,tt))
plot(tt, somme(20,tt))
plot(tt, somme(50,tt))
plot(tt, somme(200,tt))

plot(tt, (-0.495*tt + 1.55), "r")
