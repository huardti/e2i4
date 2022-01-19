#include <iostream>

#include "polynome.hpp"

#include<string>

int main(__attribute__((unused)) int argc, __attribute__((unused)) const char* argv[]) {
#if 0 // test pour les nombres complexes
    const Complexe a{1,1};
    const Complexe b{2,2};
    a.print();
    std::cout<< std::endl;
    a.print();
    Complexe c = a + b;
    c.print();
#endif
#if 1 // test pour les polynomes
    Poly2deg p(1,2,3);
    Poly2deg::solutions_t sol = p.solution();
    std::cout << p.delta() << std::endl;
    sol[0].print();
    std::cout << std::endl;
    sol[1].print();
    std::cout << std::endl;
#endif
    return 0;
}