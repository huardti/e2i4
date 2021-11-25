#%% import 
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.stats import norm
from math import sqrt
from scipy.stats import binom
#%% chargement de l'echantillon
#les pieces douvent etre de 20 cm de diametre => mu = 20

ech = pd.read_excel("Exercice1.xlsx",header=0, engine="openpyxl", verbose=True, na_values=["na"])
ech.dropna(inplace=True)

#%% Question 1 : description de l'echantillon
print("taille de l'echantillon", len(ech.values))
print("la moyenne xbarre de l'echantillon est : ", np.mean(ech.values))
print("la variance s^2 de l'echantillon est : ", np.var(ech.values))

"""
FreqRelatives, bin_edges = np.histogram(ech.values, bins=20, density=True)
plt.figure()
plt.bar(bin_edges[:-1], FreqRelatives, width=bin_edges[1]-bin_edges[0], color='red', alpha=0.5)
plt.xlabel('Taille')
plt.ylabel('Effectifs relatifs')
plt.show()
"""
# boxplot de ech
plt.figure()
plt.boxplot(ech.values)
plt.xlabel("diamètre mesuré")
plt.show()
# %% Question 2 
