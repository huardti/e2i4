function y = triangle(x)
y = zeros(x)
n = length(x)
for i=1:n,
    if(-%pi <= x(i) & x(i) <= 0) 
        then y(i) = x(i)+%pi
    elseif(0 <= x(i) & x(i) <= %pi)
        then y(i) = -x(i)+%pi
    end
end
endfunction

repmat(triangle(x))