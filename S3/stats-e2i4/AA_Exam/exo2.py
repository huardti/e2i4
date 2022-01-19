#%%
from matplotlib import colors
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.stats import norm
from math import sqrt

ech = pd.read_excel("VaccinPlacebo.xlsx",header=0, index_col=0, engine="openpyxl")

# %%
"""

print("Nombre de participants à l'essai clinique : ", len(ech.values))

# histogramme repartition homme femme
(Valeurs,FreqAbsolues)=np.unique(ech.loc[:,'Genre'], return_counts=True)
FreqRelatives=FreqAbsolues/len(ech.values)

plt.figure()
plt.bar([Valeurs[0], Valeurs[1]], FreqRelatives)
plt.xlabel('Genre')
plt.ylabel('Effectifs relatif')
plt.title("Part homme / femme de l'echantillon")
plt.show()

# histogramme repartition dans chacun des bras
(Valeurs,FreqAbsolues)=np.unique(ech.loc[:,'Traitement'], return_counts=True)
FreqRelatives=FreqAbsolues/len(ech.values)

plt.figure()
plt.bar([Valeurs[0], Valeurs[1]], FreqRelatives)
plt.xlabel('Genre')
plt.ylabel('Effectifs relatif')
plt.title("repartition actif /placebo")
plt.show()

# histogramme repartition homme femme dans chaque groupe
actif = []
placebo = []

for v in ech.values :
    if v[0] == "actif":
        actif.append(v[1])
    else:
        placebo.append(v[1])

#actif
(Valeurs,FreqAbsolues)=np.unique(actif, return_counts=True)
FreqRelatives=FreqAbsolues/len(ech.values)

plt.figure()

plt.subplot(1,2,1)
plt.bar([Valeurs[0], Valeurs[1]], FreqRelatives)
plt.xlabel('Genre')
plt.ylabel('Effectifs relatif')
plt.title("Part homme / femme dans le groupe actif")

#placebo
(Valeurs,FreqAbsolues)=np.unique(placebo, return_counts=True)
FreqRelatives=FreqAbsolues/len(ech.values)

plt.subplot(1,2,2)
plt.bar([Valeurs[0], Valeurs[1]], FreqRelatives)
plt.xlabel('Genre')
plt.ylabel('Effectifs relatif')
plt.title("Part homme / femme dans le groupe placebo")

plt.show()
"""



#3

h = []
f = []

for v in ech.values :
    if v[1] == "H":
        h.append(v[2])
    else:
        f.append(v[2])
print(h)
#homme
(Valeurs,FreqAbsolues)=np.unique(h, return_counts=True)
FreqRelatives=FreqAbsolues/len(ech.values)

plt.figure()

plt.subplot(1,2,1)
plt.bar(["non malade", "malade"], FreqRelatives)
plt.ylabel('Effectifs relatif')
plt.title("proportion de malade homme")

#femme
(Valeurs,FreqAbsolues)=np.unique(f, return_counts=True)
FreqRelatives=FreqAbsolues/len(ech.values)

plt.subplot(1,2,2)
plt.bar(["non malade", "malade"], FreqRelatives)
plt.ylabel('Effectifs relatif')
plt.title("proportion de malade femme")

plt.show()