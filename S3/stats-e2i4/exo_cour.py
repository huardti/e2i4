import numpy as np
import matplotlib.pyplot as plt

#%%
X = [332, 336, 340, 344, 348]
N = np.shape(X)[0]
# moyenne
print(np.mean(X))
# variance
print(np.var(X))

# Les données correspondent aux réalisations d'une variable aléatoire discrète
# on récupère les valeurs prises par la VAD et
# on compte le nombre de fois que le VAD prend chacune des valeurs
probas = np.zeros(np.shape(X)) + 1/N

plt.figure(1)
plt.bar(X, probas, width=0.5, color = 'blue')
plt.xlabel('Valeurs')
plt.ylabel('Effectifs relatifs')
plt.show()

ech = []
for i in X:
    for j in X:
        ech.append(  (i, j)  )

for elem in ech:
    print(elem)

Xbarre = np.mean(ech, axis=1) #moyenne de chaque paire d'ech
print(Xbarre)

Xbarre_mean = np.mean(Xbarre)
Xbarre_var = np.var(Xbarre)

print(Xbarre_mean)
print(Xbarre_var)

(Valeurs,FreqAbsolues)=np.unique(Xbarre, return_counts=True)
# on calcule à partir des effectifs absolus, les effectifs relatifs
FreqRelatives=FreqAbsolues/len(Xbarre)

plt.figure(2)
plt.bar(Valeurs, FreqRelatives, width=0.5, color = 'blue')
plt.xlabel('Valeurs')
plt.ylabel('Effectifs relatif')
plt.show()
