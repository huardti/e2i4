#%% import
import numpy as np
import pandas as pd
import scipy as sc

from scipy.stats import norm

from scipy.stats.stats import _mad_1d, f_oneway, ttest_1samp

#%% chargement de l'echantillon

ech = pd.read_excel("Data.xlsx",header=0, engine="openpyxl")

# print(ech)
# %%
vect1 = ech.loc[:,'A']
vect2 = ech.loc[:,'B']
vect3 = ech.loc[:,'C']
vect4 = ech.loc[:,'D']

# %%

m1 = np.mean(vect1)
m2 = np.mean(vect2)
m3 = np.mean(vect3)
m4 = np.mean(vect4)

print(m1)
print(m2)
print(m3)
print(m4)

Xg = (m1 + m2 + m3 + m4)/4

vtot = 0
for v in vect1:
    vtot += (v - Xg) * (v - Xg)

for v in vect2:
    vtot += (v - Xg) * (v - Xg)

for v in vect3:
    vtot += (v - Xg) * (v - Xg)

for v in vect4:
    vtot += (v - Xg) * (v - Xg)

print("variation totale = ", vtot)

print(f_oneway(vect1, vect2, vect3, vect4))

#calcul f critique

print(sc.stats.f.ppf(0.95, 3, 24))