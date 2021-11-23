import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import norm
from math import sqrt
from scipy.stats import binom

#Exercice Préliminaire
##On va etudier la loi normale de moy=20 et d'ecart type sigma=2
##population : x->N(mu,sigma²) E(x)=mu=20 V(x)=sigma²=4
##1/ generer un tirage de n=100 valeurs dans cette population xbarre et comparer
##xbarre à mu
##2/repeter cette opération 100 fois, on aura 100 valeurs de XBARRE calculer
##E(XBARRE) et V(XBARRE) et enfin tracer la distribution(hist)
##Refaire 2/ mais pour 1000 tirage ,  quel est l'effet sur XBARRE

plt.close("all")
#%%
##1/
population=[]
n=100
sigma=2
mu=20

##tirage en suivant une loi normale non centrée
print("1/")
print("1 seul tirage")
population=np.random.normal(mu,sigma,n)
xbarre=np.mean(population)
xvar=np.var(population)
print("xvar :",xvar)
print("xbarre :",xbarre)
print()
##Sur 100 tirages on observe un écart sur la moyenne (du au faible nombre de tirage)

#%%
##2/
print("2/")
print("Etude pour 100 Echantillons")
ech=[]
for i in range (0,100):
    ech.append(np.mean(np.random.normal(mu,sigma,n)))
XBARRE=np.mean(ech)
XVAR=np.var(ech)
print("XBARRE :",XBARRE)
print("XVAR :",XVAR)
print()

##Tracé de la distribution
x=np.linspace(19,21,1000)

plt.figure()
plt.hist(ech,bins=30,density=1)
plt.plot(x,norm.pdf(x,XBARRE,sqrt(XVAR)),label="pour 100 Valeurs")
plt.title("Distribution pour 100 tirages d'échantillons de 100 elements")
plt.xlabel("Valeurs")
plt.ylabel("Nombres de valeurs")
plt.legend()
plt.show()

#%%
##3/
print("3/")
print("Etude pour 1000 Echantillons")
ech2=[]
for i in range (0,1000):
    ech2.append(np.mean(np.random.normal(mu,sigma,n)))
XBARRE2=np.mean(ech2)
XVAR2=np.var(ech2)
print("XBARRE2 :",XBARRE2)
print("XVAR2 :",XVAR2)

plt.figure()
plt.hist(ech2,bins=30,density=1,color='r')
plt.plot(x,norm.pdf(x,XBARRE2,sqrt(XVAR2)),label="pour 1000 tirages d'echantillons")
plt.title("Distribution pour 1000 tirages d'échantillon de populations 100 elements")
plt.xlabel("Valeurs")
plt.ylabel("Nombres de valeurs")
plt.legend()
plt.show()
#%%
##Exo1/
##n personnes au hasard Xi=1 si A Xi=0 si B
##Xi suit une loi de bernouilli de paramaetre p inconnu.
p=np.random.uniform(0,1)
n=100 ##taille echantillon >30 docn X barre suit une loi normale

Echantillon=np.random.binomial(1,p,n) #1 car on fais des tirages independants
pestim=np.mean(Echantillon)
print("La valeur de p estimé est pour 100 tirages independants pestim= ",pestim)
print("\n")
print("La valeur réelle de p est p= ",p)

##On repete le meme process pour 1000 echantillons de n=100 tirages
F=[]
for i in range (0,1000):
    F.append(np.mean(np.random.binomial(1,p,n)))
papprox=np.mean(F)


plt.figure()
plt.hist(F,bins=30,density=1,color='r')
plt.title("Distribution pour 1000 tirages d'échantillon de populations 100 elements")
plt.xlabel("Valeurs")
plt.ylabel("Nombres de valeurs")
plt.legend()
plt.show()


##Une distribution qui forme une gaussienne autour de la valeur reelle de p.
##On observe bien la loi normale avec la distribution qui se place autour de la valeur de p

#%%
##Exo2 intervalle de confiance

def Intervalle_confiance_moyenne(ech,seuil):
    ##seuil en %

        #taille n de l'echantillon
    n=len(ech)
    #print("La taille de l'echantillon est de ",n)

        #calcul de la moyenne
    mu=np.mean(ech)
    #print("La moyenne de l'echantillon est de ",mu)

        #calcul ecart type sur ech
    ecart_type=np.std(ech)
    #print("L'ecart type est de ",ecart_type)

        #calcul de zalpha2
    val=seuil/100+(1-seuil/100)/2
    zalpha2=norm.ppf(val,0,1)
    #print(zalpha2)

        #calcul des bornes de l'intervales
    marge=2*zalpha2*ecart_type/(sqrt(n))

    return (mu-marge,mu+marge)

##test
##ech=np.random.binomial(100,0.5,50)
##print("intervalle de confiance moyenne population",Intervalle_confiance_moyenne(ech, 95))

def Intervalle_confiance_proportion(ech,seuil):
    ##seuil en %

        #taille n de l'echantillon
    n=len(ech)
    #print("La taille de l'echantillon est de ",n)

        #calcul de la moyenne
    mu=np.mean(ech)
    #print("La moyenne de l'echantillon est de ",mu)

        #calcul ecart type sur ech
    ecart_type=np.std(ech)
    #print("L'ecart type est de ",ecart_type)

        #calcul de zalpha2
    val=seuil/100+(1-seuil/100)/2
    zalpha2=norm.ppf(val,0,1)
    #print(zalpha2)

        #calcul des bornes de l'intervales
    marge=zalpha2*(sqrt((mu*(1-mu))/n))


    return (mu-marge,mu+marge)

##test
##ech2=np.random.binomial(1,0.5,500)
##print("intervalle de confiance moyenne de la proportion ",Intervalle_confiance_proportion(ech2, 95))


#%%
##Exo3
##taille de l'echantillon n
n=100
mu=50
sigma=5
nb_mu_in_int=0;
for i in range(0,100):
    ech3=(np.random.normal(mu,sigma,n))
    tempint=Intervalle_confiance_moyenne(ech3, 95)
    if(tempint[0]<=mu<=tempint[1]):
        nb_mu_in_int+=1

print("Sur 100 tirages, il y'a ",nb_mu_in_int," de fois ou mu est dans l'intervalle")


