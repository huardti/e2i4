#%% import 
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

from scipy.stats import norm
from math import sqrt

from scipy.stats.stats import ttest_1samp

#%% chargement de l'echantillon

ech = pd.read_excel("Exercice1.xlsx",header=0, engine="openpyxl", index_col=0, verbose=True)

print("taille de l'echantillon", len(ech.values))
print("la moyenne xbarre de l'echantillon est : ", np.mean(ech.values))
print("la variance s^2 de l'echantillon est : ", np.var(ech.values, ddof=0)) # ddof car on calcul la variance sur un echantillon
print("L'ecart type dans cet echantillon est de : ",np.std(ech.values, ddof=0))
#%% calcul de la norm

from scipy.stats import norm
alpha = 0.05
print("la valeur de la normal : ", norm.ppf(1-(alpha/2)))

#%% test  hypothese
from scipy.stats import ttest_ind

print(ttest_1samp(ech.values, 27))


# test avec stats models
# import statsmodels as sm
# print(sm.ztest(ech.values, value=27))

#test de normalité de shapiro
from scipy.stats import shapiro
sp = shapiro(ech.values)
print(sp)