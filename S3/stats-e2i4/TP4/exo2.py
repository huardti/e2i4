#%% import
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.stats import norm

#%% chargement de l'echantillon
#les pieces douvent etre de 20 cm de diametre => mu = 20

ech = pd.read_excel("Exercice2.xlsx",header=0, engine="openpyxl", index_col=0, verbose=True)

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

# %%
