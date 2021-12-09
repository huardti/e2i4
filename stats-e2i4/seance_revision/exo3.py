#%% import
import numpy as np
import pandas as pd
import scipy as sc
import matplotlib.pyplot as plt

#%% chargement des echantillons

ech = pd.read_excel("Exercice3.xlsx",header=0, engine="openpyxl", verbose=True)

# print(ech)
# %% description des echantillons
vect1 = ech.iloc[:,0]
vect2 = ech.iloc[:,1]
vect3 = ech.iloc[:,2]
# %%
plt.figure()
# boxplot du programme 1
plt.subplot(1,3,1)
plt.boxplot(vect1)
plt.xlabel("temps d'execution")

# boxplot du programme 2
plt.subplot(1,3,2)
plt.boxplot(vect2)
plt.xlabel("temps d'execution")

# boxplot du programme 3
plt.subplot(1,3,3)
plt.boxplot(vect3)
plt.xlabel("temps d'execution")

plt.show()

print("moyenne X1 = ", np.mean(vect1))
print("moyenne X2 = ", np.mean(vect2))
print("moyenne X3 = ", np.mean(vect3))
print()
print("Variance X1 = ", np.var(vect1))
print("Variance X2 = ", np.var(vect2))
print("Variance X3 = ", np.var(vect3))
print()
print("ecart type X1 = ", np.std(vect1))
print("ecart type X2 = ", np.std(vect2))
print("ecart type X3 = ", np.std(vect3))