// Exo 5.1 part 1
A = ones(4,4)
A(3, 1:2) = 0

B = eye(4,4)
B(:,4) = 1

C = eye(4,4)
C(:,1) = 1

// Exo 5.1 part 2
D = 2*eye(100,100) + diag(ones(1,99),1)
