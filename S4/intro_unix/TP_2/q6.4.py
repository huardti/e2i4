from pprint import pprint

def fact(n):
    fact = 1
    while(n != 0):
        fact *= n
        n=n-1
    return fact

def anagramme_pos(ch, pos):
    return (ch[pos] + ch[pos-2] + ch[pos-1], ch[pos] + ch[pos-1] + ch[pos-2])

def est_dans_tableau(tab, ch):
    return ch in tab

def remplir_tab_anagramme(tab,ch):
    for i in range(0, len(ch)):
        tab.append()
    return fact(len(ch))

ch = "c"
tab = ["abc", "aaa", "bbb"]

print("in = ", est_dans_tableau(tab, ch))
print(tab)