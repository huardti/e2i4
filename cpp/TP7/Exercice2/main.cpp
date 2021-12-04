int main(int argc, const char* argv[])
{
    Cercle cercle(10,10,4);
    cout <<endl<<cercle<<" surface="<<cercle.surface()<<endl;
    Triangle triangle(20,20,3);
    cout<<endl<<triangle<<" surface="<<triangle.surface()<<endl;
    Rectangle rectangle(30,30,2,5);
    cout << endl <surface() << endl << endl;
    cercle.deplace(50,50);
    cout << "déplacement " << endl;
    cout << cercle << endl << endl;
    *********;
    //déclaration du tableau initialisé ci-dessous ???
    tab[0] = &cercle;
    tab[1] = &triangle;
    tab[2] = &rectangle;
    tab[3] = carre;
    float surf=0.0;
    for (int i=0; i<4; i++) {
    surf += *****************;
    }
    cout << "surface totale : " << surf << endl << endl;
    cout << "périmètre d'une forme tirée au hasard" << endl;
    srand((unsigned int) time(NULL));
    ****** ptr = tab[rand()%4]; // définition de ptr
    cout << **** << " périmètre=" << ******** << endl << endl;
    cout << "destruction de carré alloué dynamiquement" << endl;
    ptr = carre; delete ptr; cout << endl;
    return 0;
}

/* execution trace */
#if 0

- Forme::Forme -- Cercle::Cercle -
Cercle (10,10) r=4 surface=50.2655
- Forme::Forme -- Triangle::Triangle -
Triangle (20,20) c=3 surface=4.5
- Forme::Forme -- Rectangle::Rectangle -
Rectangle (30,30) L=2 l=5 surface=10
- Forme::Forme -- Rectangle::Rectangle -- Carre::Carre -
Carre (100,100) c=2 surface=4
déplacement
Cercle (60,60) r=4
surface totale : 68.7655
périmètre d'une forme tirée au hasard
Carre (100,100) c=2 périmètre=8
destruction de carre alloué dynamiquement
- Carre::~Carre -- Rectangle::~Rectangle -- Forme::~Forme -
- Rectangle::~Rectangle -- Forme::~Forme -
- Triangle::~Triangle -- Forme::~Forme -
- Cercle::~Cercle -- Forme::~Forme -

#endif