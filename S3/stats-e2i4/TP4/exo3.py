#%% import
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.stats import norm

#%% chargement de l'echantillon
#les pieces douvent etre de 20 cm de diametre => mu = 20

ech = pd.read_excel("Exercice3.xlsx",header=0, engine="openpyxl", verbose=True)

#%%
vect1 = []
vect2 = []
for v in ech.values :
    if v[0] == 1:
        vect1.append(v[1])
    else:
        vect2.append(v[1])
#%% Question 1 : description de l'echantillon
print("taille de l'echantillon", len(ech.values))
print("la moyenne xbarre de l'echantillon est : ", np.mean(ech.values))
print("la variance s^2 de l'echantillon est : ", np.var(ech.values))
print("L'ecart type dans cet echantillon est de : ",np.std(ech.values))

# histogramme
(Valeurs,FreqAbsolues)=np.unique(ech, return_counts=True)
FreqRelatives=FreqAbsolues/len(ech)
plt.figure()
plt.bar(['Conforme', 'Défectueux'], FreqRelatives, )
plt.xlabel('Valeurs')
plt.ylabel('Effectifs relatif')
plt.show()

plt.figure()
plt.pie(FreqRelatives, labels=['Conforme', 'Défectueux'])
plt.show()

## Tracé de la distribution
x=np.linspace(195,205,479)
plt.figure()
plt.hist(ech,bins=30,density=1)
plt.plot(x,norm.pdf(x,np.mean(ech.values),np.std(ech.values)),label="pour 479 Valeurs")
plt.title("Distribution des diamètres comparé à une distribution normale")
plt.xlabel("Diamètres mesurés")
plt.ylabel("Nombres de mesures")
plt.legend()
plt.show()
# on voit bien un distribution normale.
# %%
