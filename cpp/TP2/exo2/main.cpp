#include <iostream>

#include "pile.hpp"

int main(int argc, const char* argv[]) {
    Pile pile(3);
    pile.empiler(1);
    pile.empiler(2);
    pile.empiler(3);
    pile.empiler(4);
    pile.afficher();
    std::cout << "nombre d'element dans la pile : " << pile.nb_elements() << std::endl;

    std::cout << "nombre de pile : " << pile.nb_pile() << std::endl;
    Pile pie(10);
    std::cout << "nombre de pile : " << pile.nb_pile() << std::endl;

    return 0;
}
