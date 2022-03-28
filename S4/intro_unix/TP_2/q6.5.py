def compter_mot(ch):
    tmp = ch.split()
    return len(tmp)

inp = "je crois ce que je dis et je fais ce que je crois"

w_list = inp.strip().split()
dico = {item : w_list.count(item) for item in w_list}
l = []
for item in dico:
    l.append({item: dico[item]})

print(dico)
print(l)
%