#include <iostream>

#include "forme.hpp"

#include "cercle.hpp"
#include "triangle.hpp"
#include "rectangle.hpp"
#include "carre.hpp"

int main()
{
    Cercle cercle(10,10,4);
    std::cout <<std::endl <<cercle<<" surface="<<cercle.surface()<<std::endl;
    Triangle triangle(20,20,3);
    std::cout<<std::endl<<triangle<<" surface="<<triangle.surface()<<std::endl;
    Rectangle rectangle(30,30,2,5);
    std::cout<<std::endl<<rectangle<<" surface="<<rectangle.surface()<<std::endl;
    Carre *carre = new Carre(30,30,2);
    std::cout<<std::endl<<(*carre)<<" surface="<<carre->surface()<<std::endl;

    cercle.deplace(50,50);
    std::cout << "déplacement " << std::endl;
    std::cout << cercle << std::endl << std::endl;

    Forme* tab[4];
    tab[0] = &cercle;
    tab[1] = &triangle;
    tab[2] = &rectangle;
    tab[3] = carre;
    float surf=0.0;
    for (int i=0; i<4; i++) {
    surf += tab[i]->surface();
    }
    std::cout << "surface totale : " << surf << std::endl << std::endl;
    std::cout << "périmètre d'une forme tirée au hasard" << std::endl;
    srand((unsigned int) time(NULL));
    Forme* ptr = tab[rand()%4]; // définition de ptr
    std::cout << (*ptr) << " périmètre=" << ptr->perimetre() << std::endl << std::endl;
    std::cout << "destruction de carré alloué dynamiquement" << std::endl;

    ptr = carre;
    delete ptr;
    std::cout << std::endl;
    return 0;
}