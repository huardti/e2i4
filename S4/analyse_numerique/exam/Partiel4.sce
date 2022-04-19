function res=f1(t,y)
    res=-10*y+50;
endfunction


function [t,y]=euler_implicite(t0,y0,T,N,f)
    h=T/N
    t=[t0]
    y=[y0]
    for i=1:N
        t(i+1)=t(i)+h
        function out=g(in)
            out=y(i)+h*f(t(i+1),in)-in
        endfunction
        y(i+1)=fsolve(y(i),g)
    end
endfunction

function test()
        T=1
        N=250

        t0=0
        y0=0
        
        [t,y]=euler_implicite(t0,y0,T,N,f1)
        plot(t,y,"red")
        xlabel("temps (s)");
        ylabel("tension (V)");
        legend("euler implicite")
        
        yth = 5*(1-exp(-t/0.1))
        plot(t,yth)
        legend("euler implicite", "solution analytique")
endfunction

test()
