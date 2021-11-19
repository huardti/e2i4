#include <iostream>
#include "string1.hpp"

int main() {
    String ch1("essai");
    String ch2 = ch1;
    String ch3('=' , 3);
    const String ch4("chaîne constante");
    ch1.nieme(1) = 'E'; // le premier caractère de la chaîne
    std::cout << ch4.nieme(1) << std::endl;
    ch2.saisie();
    ch2.concatene(" de la classe String");
    ch2.affiche();
    ch2.concatene('g');
    ch2.affiche();
#if 0
    if ( ! egal(ch2, "") ) {
     ch2.affiche();
     cout << endl;
    }
    ch2.minuscule().affiche();// est-ce bien raisonnable ???
    cout << endl;
#endif
    return 0;
}