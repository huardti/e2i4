import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import binom

plt.close("all")

Ech = np.loadtxt('./PiecesDef.txt')
print("la taille est", np.shape(Ech))
    # on peut aussi utiliser len

valeurs, effectif = np.unique(Ech, return_counts=True)
effectif_relatif = effectif / len(Ech)

effectif_relatif_cum = np.cumsum(effectif_relatif)
print("valeurs\t\t",valeurs)
print("relatif\t\t", effectif)
print("effectif cumulé\t", effectif_relatif_cum)

plt.bar(valeurs - 0.05,effectif_relatif, width= 0.1, color = 'red')

plt.xlabel("valeurs")
plt.ylabel("effectifs relatifs")

p = (sum(valeurs*effectif) / (40 * len(Ech)))
freq_relative_thoe = binom.pmf(valeurs, 40, p)
plt.bar(valeurs+0.05, freq_relative_thoe, width=0.1, color = "blue")
plt.show()