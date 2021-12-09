#%% import 
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.stats import norm
from math import sqrt

from scipy.stats.stats import ttest_1samp

#%% chargement de l'echantillon
#les pieces douvent etre de 20 cm de diametre => mu = 20

ech = pd.read_excel("Exercice1.xlsx",header=0, engine="openpyxl", verbose=True, na_values=["na"])
ech.dropna(inplace=True)

#%% Question 1 : description de l'echantillon
print("taille de l'echantillon", len(ech.values))
print("la moyenne xbarre de l'echantillon est : ", np.mean(ech.values))
print("la variance s^2 de l'echantillon est : ", np.var(ech.values))
print("L'ecart type dans cet echantillon est de : ",np.std(ech.values))

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

## Tracé de la distribution
x=np.linspace(195,205,479)
plt.figure()
plt.hist(ech,bins=30,density=1)
plt.plot(x,norm.pdf(x,np.mean(ech.values),np.std(ech.values)),label="pour 479 Valeurs")
plt.title("Distribution des diamètres comparé à une distribution normale")
plt.xlabel("Diamètres mesurés")
plt.ylabel("Nombres de mesures")
plt.legend()
plt.show()
# on voit bien un distribution normale.



#%% Question 2 /3 Calculs de l'intervalle de confiance à 95%
#Expliquer ce qu'on voit

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

    return (xbarre-marge/2,xbarre+marge/2)

print("intervalle de confiance moyenne population",Intervalle_confiance_moyenne(ech.values, 95,1))

##Question 4 : Faire le test des hypothèses
from scipy.stats import ttest_ind

print(ttest_1samp(ech.values, 200))
