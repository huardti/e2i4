#include <iostream>

/* // exo string TP 4

#include "string1.hpp"
int main() {
    String ch1("essai");
    String ch2 = ch1;
    String ch3('=' , 80);
    const String ch4("chaîne constante");
    ch1.nieme(1) = 'E'; // le premier caractère de la chaîne
    std::cout << ch4.nieme(1) << std::endl;
    ch2.saisie();
    std::cout << std::endl;
    ch2.concatene(" de la classe String");
    ch2.concatene('g');
    std::cout << egal(ch2, "") << std::endl;
    if ( !egal(ch2, "") ) {
        std::cout << "different" << std::endl;
        ch2.affiche();
        std::cout << std::endl;
    }
    ch2.minuscule().affiche();// est-ce bien raisonnable ???
    std::cout << std::endl;
    return 0;
}
*/
#include "string2.hpp"


int main() {
    String2 ch1("essai");
    String2 ch2 = ch1;
    String2 ch3('=', 80);
    const String2 ch4("chaîne de caractères constante");
    ch1[1] = 'E';
    // le premier caractère de la chaîne
    std::cout << ch4[1] << std::endl;
    ch1 = "<<<< " + ch2 + " >>>>";
    std::cout << ch1 << std::endl;
    std::cin >> ch2;
    ch2 += " de la classe Strin";
    ch2 += 'g';
    if ( ch2 != "" ) std::cout << ch2 << std::endl;
    std::cout << ch2.minuscule() << std::endl;
    return 0;
}