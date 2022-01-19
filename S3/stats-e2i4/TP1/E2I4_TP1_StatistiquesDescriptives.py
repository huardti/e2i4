# -*- coding: utf-8 -*-
"""
Created on Wed Feb 10 19:11:50 2021

@author: guyadern
"""
#%%

# Exercice1

# pour supprimer toutes les variables de l'explorateur
from IPython import get_ipython
# get_ipython().magic('reset -sf')

# On importe les librairies dont on a besoin
import numpy as np
import matplotlib.pyplot as plt


# pour fermer toutes les figures au début du programme
plt.close('all')

# Récupération des données
Ech = np.loadtxt('PiecesDef.txt')

# Les données correspondent aux réalisations d'une variable aléatoire discrète
# on récupère les valeurs prises par la VAD et 
# on compte le nombre de fois que le VAD prend chacune des valeurs
(Valeurs,FreqAbsolues)=np.unique(Ech, return_counts=True)
# on calcule à partir des effectifs absolus, les effectifs relatifs
FreqRelatives=FreqAbsolues/len(Ech)
# on calcule les effectifs relatifs cumulés
FreqRelativesCum=np.cumsum(FreqRelatives)

# toutes ces valeurs correspondent au tableau de synthèse des données d'une VAD
# que l'on retrouve dans l'exemple du cours

# on peut maintenant visualiser la distribution de la VAD 
# comme la VA est discrète on utilise une représentation en forme de diagramme en bâtons 
plt.figure(1)
plt.bar(Valeurs-0.1,FreqRelatives, width = 0.1, color = 'red', label='Freq relatives obs')
#plt.xlim(-1,9)
#plt.ylim(0,0.3)
plt.xlabel('Valeurs')
plt.ylabel('Effectifs relatifs')
plt.show()

# on peut également visualiser les effectifs relatifs cumulés
plt.figure(2)
plt.step(Valeurs,FreqRelativesCum)
plt.xlabel('Valeurs')
plt.ylabel('Effectifs relatifs cumulés')
plt.show()


# On s'intéresse dans un second temps à la loi binomiale
# loi qui pourrait correspondre à celle suivie par le VAD étudiée 
# car nombre de pièces défecteuses parmi 40 
# on estime la proportion de pièces défectueuses, c'est-à-dire le paramètre de la loi de Bernouilli


# Soit X la variable aléatoire de Bernouilli de paramètre p
# X = 1 si piece defectueuse et X = 0 sinon
p=sum(Valeurs*FreqAbsolues)/(40*len(Ech))

# Soit Y la variable binomiale associée de parametres n=40 et p
# Y nombre de pieces defectueuse sur un lot de 40 

from scipy.stats import binom
# on calcule les freq relatives theoriques
FreqRelativesTheo=binom.pmf(Valeurs,40,p)
plt.bar(Valeurs+0.1,FreqRelativesTheo, width = 0.1, color = 'blue', label='Freq relatives theo')
plt.legend(loc='best')
plt.show()


#%%
# Exercice 2: Loi normale
# pour supprimer toutes les variables de l'explorateur
from IPython import get_ipython
get_ipython().magic('reset -sf')

# On importe les librairies dont on a besoin
import numpy as np
import matplotlib.pyplot as plt

# pour fermer toutes les figures au début du programme
plt.close('all')

# on génére des nombres aleatoires issus de la loi normale centrée réduite
vect=np.random.randn(100);
# On peut aussi utiliser
#moy=0;sigma=1;vect=np.random.normal(moy,sigma**2,100)

# on calcule la moyenne, l'écart-type et la variance
Moy=np.mean(vect)
EcartType=np.std(vect)
Variance=np.var(vect)

# Exemple de ligne de code qui refait les calculs
Moy2=sum(vect)/len(vect);
Variance2=sum((vect-Moy)**2)/len(vect)

# on trace l'histogramme
plt.figure()
plt.hist(vect,bins=10,density=1)
plt.show()

# on peut aussi récupérer les valeurs qui correspondent à l'histogramme
FreqRelatives, bin_edges = np.histogram(vect, bins=10, density=True)
plt.figure()
plt.bar(bin_edges[:-1], FreqRelatives, width=bin_edges[1]-bin_edges[0], color='red', alpha=0.5)
plt.xlabel('Valeurs')
plt.ylabel('Effectifs relatifs')
plt.show()

# on calcule les fréquences relatives cumulées
dx = bin_edges[1] - bin_edges[0]
FreqRelativesCum = np.cumsum(FreqRelatives)*dx

# et on les trace
plt.figure()
plt.plot(bin_edges[:-1], FreqRelativesCum)
plt.xlabel('Valeurs')
plt.ylabel('Effectifs relatifs cumulés')
plt.show()

# on trace le boxplot de la VAC
plt.figure()
plt.boxplot(vect)
plt.ylabel('Quartiles et médiane de la VAC')
plt.show()

# on peut directement avoir les valeurs: médiances et quartiles
print('la médiane est:', np.median(vect))
print('Les quartiles sont:', np.quantile(vect,[0.25, 0.5, 0.75]))



# Dans la deuxième partie de l'exercice on génère un vecteur de nombres aléatoires
# suivant une loi normale de moyenne 14 et d'écart-type 3
from scipy.stats import norm

# on génère ici un très grand nombre de valeurs pour approcher la loi théorique
moy=14;ecart=3;
vect2=np.random.normal(moy,ecart,10000)

x= np.linspace(np.min(vect2),np.max(vect2),10000)
# on va alors tracer la distribution et la fonction de répartition de la variable observée
# et tracer en parallèle la densité de probabilité de la loi normale ainsi que sa fonction de répartitio
FreqRelatives2, bin_edges2 = np.histogram(vect2, bins=50, density=True)

plt.figure()
plt.bar(bin_edges2[:-1], FreqRelatives2, width=bin_edges2[1]-bin_edges2[0], color='red', alpha=0.5)
plt.plot(x,norm.pdf(x,moy,ecart))
plt.xlabel('Valeurs')
plt.ylabel('Effectifs relatifs')
plt.show()

dx2 = bin_edges2[1] - bin_edges2[0]
FreqRelativesCum2 = np.cumsum(FreqRelatives2)*dx2

plt.figure()
plt.plot(bin_edges2[:-1], FreqRelativesCum2)
plt.plot(x,norm.cdf(x,moy,ecart))
plt.xlabel('Valeurs')
plt.ylabel('Effectifs relatifs cumulés')
plt.show()


# on pouvait également tracer la densité de probabilité :
y=(np.exp(-((x-moy)/ecart)**2/2))/np.sqrt(2*np.pi*ecart**2)
plt.figure()
plt.plot(x,y)


# on répond enfin à la dernière question de l'exercice: probabilité que la variable
# soit comprise entre 11 et 17
print(norm.cdf(17,moy,ecart)-norm.cdf(11,moy,ecart))

#%%
# Exercice 3

# La librairie Panda permet de gérer de nombreux formats de données
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt 

plt.close('all')

# On charge les données
X = pd.read_excel("Exercices1.xlsx",sheet_name=0,header=0,index_col=0, engine="openpyxl")
# On peut ravailler directement sur les dataframe
# ou convertir les dataframe en array 

# Partie 1 : array
# On travaille avec les arrays
XArray=pd.DataFrame.to_numpy(X)

# on vérifie s'il y a des données manquantes
XArray2=XArray[~np.isnan(XArray).any(axis=1)]

# On calcule Moyenne et Ecarttype des variables  
# On calcule la moyenne des variables Height et Weight
np.mean(XArray2[:,0:2],axis=0)
np.mean(XArray2[:,8:10],axis=0)
# On realise des boxplot des variables quantitatives
plt.figure()
plt.boxplot(XArray2[:,(0,1,8,9)])
plt.xticks([1, 2, 3, 4], ['Height', 'Weight', 'Pulse1', 'Pulse2'])
plt.show()


# On compte le nombre de modalités des variables categorielles
for i in range(5):
    print(np.unique(XArray2[:,i+3], return_counts=True))
    print(i)

# On trace le graphique Height en fonction de Weight
plt.figure()
plt.style.use('ggplot')
plt.scatter(XArray[:,0],XArray[:,1])
plt.show()

# On calcule la corrélation entre Height et Weight
print(np.corrcoef(XArray2[:,0],XArray2[:,1]))

# On peut maintenant s'amuser à regarder 
plt.figure()
plt.style.use('ggplot')
plt.scatter(XArray[:,8],XArray[:,9])
plt.show()
# On calcule la corrélation entre Height et Weight
print(np.corrcoef(XArray[:,8],XArray[:,9]))

# on regarde la même figure en separant en fonction de Ran
Ran1=(XArray[:,7]==1)
Ran2=(XArray[:,7]==2)
plt.figure()
plt.style.use('ggplot')
plt.scatter(XArray[Ran1,8],XArray[Ran1,9],marker='^')
plt.scatter(XArray[Ran2,8],XArray[Ran2,9],marker='o')
plt.show()

#%%
# on recommence en utilisant les dataframes: X
# Partie 2 : dataframe

from IPython import get_ipython
get_ipython().magic('reset -sf')

# La librairie Panda permet de gérer de nombreux formats de données
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt 

plt.close('all')

# On charge les données
X = pd.read_excel("Exercices1.xlsx", sheet_name=0, header=0, index_col=0, engine="openpyxl")
# on commence comme précédemment par le retrait des lignes (c'est-à-dire des observations
# qui contiennent des nan)
X2 = X.dropna()

plt.figure()
X2.boxplot()
plt.show()

# on pourra également utiliser les fonctions
X2.plot.scatter(x='Height',y='Weight')

#X2.corr(method='pearson')
# on calcule la corrélation entre 2 variables
print('La correlation entre Height et Weight est de :', X2['Height'].corr(X2['Weight']))

# On trace maintenant les mêmes données mais avec un code couleur qui dépend de la variable
# catégorielle Ran
X2.plot.scatter(x='Height',y='Weight',c='Ran',colormap='viridis')
# %%
