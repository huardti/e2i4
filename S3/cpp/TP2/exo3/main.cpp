#include <iostream>

#include "point.hpp"

int main(int argc, const char* argv[]) {
    Point p(10, 154);
    std::cout << p.abscisse() << ", " << p.ordonnee() << ", " << p.rho() << ", " << p.theta() << std::endl;
    p.rotation(0.3f);
    std::cout << p.abscisse() << ", " << p.ordonnee() << ", " << p.rho() << ", " << p.theta() << std::endl;
    return 0;
}
