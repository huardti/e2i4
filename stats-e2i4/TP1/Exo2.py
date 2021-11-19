#%%
#partie 1
# %reset -f
import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import norm

vect = np.random.normal(size=100)

moyenne = np.mean(vect) #  sum(vecteur / len(vect))
ecart_type = np.std(vect)
variance = np.var(vect) # sum((vect-moyenne) **2) / len(vect)

plt.figure(2)
plt.hist(vect, bins=20, density=1)
plt.xlabel("values")
plt.ylabel("density")
plt.show()


# boite a moustache
plt.boxplot(vect)
plt.show()

np.median(vect)
np.quantile(vect, [0.25, 0.5, 0.75])

# densité de proba
x = np.linspace(0, 30, 10000)
moyenne = 14
ecart = 3
plt.figure()
plt.plot(x, norm.pdf(x, moyenne, ecart))
plt.title("Densité de proba loi normale M=14, Sigma=3")
plt.xlabel("valeurs")
plt.ylabel("densité")
plt.show()

plt.figure()
plt.plot(x, norm.cdf(x, moyenne, ecart))
# plt.xlabel("valeurs")
# plt.ylabel("densité")
plt.title("fonction de repartition loi normale M=14, Sigma=3")

plt.show()

print("valeur pour P(x < 0.8)", norm.cdf(0.8, 0, 1))
print("valeur pour P(-1.96 < x < 1.96)", norm.cdf(1.96, 0, 1) - norm.cdf(-1.96, 0, 1))


plt.figure()
moyenne = 14
sigma = 3
vect = np.random.normal(moyenne, sigma**2, 100)
# plt.figure(2)
plt.hist(vect, bins=15, density=1)
plt.show()
# %%
