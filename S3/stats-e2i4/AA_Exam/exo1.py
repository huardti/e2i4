#%%

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.stats import norm
from math import sqrt

# from scipy.stats.stats import ttest_1samp , ttest_ind
#%%
ech = pd.read_excel("DureeVie_Piles.xlsx",header=0, engine="openpyxl")
# %%
#descriptions des donnees

print("Laille de l'echantillon", len(ech.values))
print("La durée de vie moyenne obtenue avec cette echantillon est : ", np.mean(ech.values))
print("la variance s^2 de l'echantillon est : ", np.var(ech.values))

## Tracé de la distribution
x=np.linspace(100,250,1000)
plt.figure()
plt.hist(ech,bins=30,density=1)
plt.plot(x,norm.pdf(x,np.mean(ech.values),np.std(ech.values)))
plt.title("Distribution ")
plt.xlabel("Durée de vie des piles (heures)")
plt.ylabel("Effectif relatif")
plt.legend()
plt.show()

#%%

def Intervalle_confiance_moyenne(ech,seuil,ecart_type):
    ##seuil en %
        #taille n de l'echantillon
    n=len(ech)
        #calcul de la moyenne
    xbarre=np.mean(ech)
        #calcul de zalpha2
    val=seuil/100+(1-seuil/100)/2
    zalpha2=norm.ppf(val)
        #calcul des bornes de l'intervales
    marge=2*zalpha2*ecart_type*ecart_type/(sqrt(n))
    print("Marge d'erreur de l'intervalle de confiance :",marge)

    return (xbarre-marge,xbarre+marge)

print("intervalle de confiance moyenne population",Intervalle_confiance_moyenne(ech.values, 98,1))