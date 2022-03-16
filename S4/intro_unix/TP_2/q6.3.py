def somme_div(n):
    somme = 0
    for div in range(1, n-1):
        if((n % div) == 0):
            somme += div
    return somme

def intouchable(n):
    for i in range(2, n*n):
        if(n == somme_div(i)):
            return False
    return True

nombre = 9

x = int(input("valeur de x [1,125]: "))
print("intouchable = ", intouchable(x))