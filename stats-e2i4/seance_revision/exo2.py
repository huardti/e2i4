#%% import 
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

from scipy.stats import norm
from math import sqrt

from scipy.stats.stats import ttest_1samp

#%% chargement de l'echantillon

ech = pd.read_excel("Exercice2.xlsx",header=0, engine="openpyxl", verbose=True, na_values=["nan"])
ech.dropna()

print("tube 1")
print("taille de l'echantillon", len(ech.loc[:,'tube_ech1']))
print("la moyenne xbarre de l'echantillon est : ", np.mean(ech.loc[:,'tube_ech1']))
print("la variance s^2 de l'echantillon est : ", np.var(ech.loc[:,'tube_ech1'], ddof=1)) # ddof car on calcul la variance sur un echantillon
print("L'ecart type dans cet echantillon est de : ",np.std(ech.loc[:,'tube_ech1'], ddof=1))

print("tube 2")
print("taille de l'echantillon", len(ech.loc[:,'tube_ech2']))
print("la moyenne xbarre de l'echantillon est : ", np.mean(ech.loc[:,'tube_ech2']))
print("la variance s^2 de l'echantillon est : ", np.var(ech.loc[:,'tube_ech2'], ddof=1)) # ddof car on calcul la variance sur un echantillon
print("L'ecart type dans cet echantillon est de : ",np.std(ech.loc[:,'tube_ech2'], ddof=1))

#%% calcul de la norm

from scipy.stats import norm
alpha = 0.05
print("la valeur de la normal : ", norm.ppf(1-(alpha/2)))

#%% test  hypothese
from scipy.stats import ttest_ind

vect1 = ech.loc[:,'tube_ech1']
vect2 = ech.loc[:,'tube_ech2']

print(ttest_ind(vect2, vect1, equal_var=0)) # ??