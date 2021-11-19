#
# On va etudier la lois normale de µ=20 et d'ecart type=2
#
# population:
# 	X-> N(µ, sigma^2)
# 	E(X) = µ = 20	V(X)=2^2
#
# 1) generer un tirage de n=100 valeurs dans cette population /X
#       et comparer /X à µ.
#
# 2) repeter cette operation 100 fois
#       on a doc 100 valeurs de /X,
#       calculer E(/X), V(/X) et tracer la distribution
#
# 3)refaire 2) mais 1000 foisn quelle est l'effet sur /X ?

import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import norm

nb_x=100
nb_op  = 1000
bins_size = 40

moy=20
sigma=2

X_bar = []

for i in range(0,nb_op):
    X=np.random.normal(moy,sigma,nb_x)

    E_X = np.mean(X)
    V_X = np.var(X)

    X_bar.append(E_X)

print("moyenne : ", np.mean(X_bar), ", variance : ", np.var(X_bar))

# FreqRelatives, bin_edges = np.histogram(X_bar, bins=bins_size, density=True)
# plt.figure()
# plt.bar(bin_edges[:-1], FreqRelatives, width=bin_edges[1]-bin_edges[0], color='red', alpha=0.5)
# plt.xlabel('Valeurs')
# plt.ylabel('Effectifs relatifs')
# plt.show()

x= np.linspace(np.min(X_bar),np.max(X_bar),nb_op)

plt.figure()
plt.hist(X_bar,bins=bins_size,density=True)
plt.plot(x, norm.pdf(x,np.mean(X_bar),np.sqrt(np.var(X_bar))))
plt.xlabel('Valeurs')
plt.ylabel('Effectifs relatifs')
plt.show()

'''
plt.figure()
plt.bar(bin_edges[:-1], FreqRelatives, width=bin_edges[1]-bin_edges[0], color='red', alpha=0.5)
plt.plot(x,norm.pdf(x,moy,sigma))
plt.xlabel('Valeurs')
plt.ylabel('Effectifs relatifs')
plt.show()
'''