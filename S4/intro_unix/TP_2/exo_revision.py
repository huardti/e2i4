"""
ch = input("votre chaine de caractere: ")
tab = ch.split()
print("le plus long mot est a ", max(len(w)for w in tab), "caractere")
"""

"""
def copie_m1_m2(M1, M2, NBL, NBC):
    for i in range(0, NBL):
        for j in range(0, NBC):
            print(NBL- i,NBC-j)
            M2[NBC-1 - j][NBL-1 - i] = M1[i][j]
    return M2

NBL = 3
NBC = 4

m1 = [[1, 2, 3, 4],
    [5, 6, 7, 8],
    [9,10,11,12]]

m2 = [[0,0,0],
    [0,0,0],
    [0,0,0],
    [0,0,0]]

copie_m1_m2(m1, m2, NBL, NBC)
print(m2)
"""

