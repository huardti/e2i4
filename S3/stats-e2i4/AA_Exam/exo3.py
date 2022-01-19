#%%
from matplotlib import colors
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.stats import norm
from math import sqrt

ech = pd.read_excel("ProgType.xlsx",header=0, engine="openpyxl")



plt.figure()
plt.subplot(1,3,1)
plt.hist(ech.loc[:,'ProgType1'],bins=20,density=1, color='blue')
plt.xlabel("temps d'execution")
plt.ylabel("effectif relatif")
plt.title("programme 1")
plt.subplot(1,3,2)
plt.hist(ech.loc[:,'ProgType2'],bins=20,density=1, color='red')
plt.xlabel("temps d'execution")
plt.title("programme 2")
plt.subplot(1,3,3)
plt.hist(ech.loc[:,'ProgType3'],bins=20,density=1, color='green')
plt.xlabel("temps d'execution")
plt.title("programme 3")
plt.show()

print("programme 1 : ")
print("La taille de l'echantillon", len(ech.loc[:,'ProgType1']))
print("Temps d'execution moyen : ", np.mean(ech.loc[:,'ProgType1']))
print(ech.loc[:,'ProgType1'])
print("variance : ", np.var(ech.loc[:,'ProgType1']))

print("programme 2 : ")
print("La taille de l'echantillon", len(ech.loc[:,'ProgType2']))
print("Temps d'execution moyen : ", np.mean(ech.loc[:,'ProgType2']))
print("variance : ", np.var(ech.loc[:,'ProgType2']))

print("programme 3 : ")
print("La taille de l'echantillon", len(ech.loc[:,'ProgType3']))
print("Temps d'execution moyen : ", np.mean(ech.loc[:,'ProgType3']))
print("variance : ", np.var(ech.loc[:,'ProgType3']))