#include <iostream>

int nbjour (int mois, int annee)
{
    // trouve si l'annee est bissextile
    switch (mois)
    {
    // mois a 31 jours
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        return 31;
        break;

    // mois a 30 jours
    case 4:
    case 6:
    case 9:
    case 11:
        return 30;
        break;

    // fevrier
    case 2:
        if(annee % 4 == 0)
        {
            if(annee % 400 == 0) { return 29; }
            else if(annee % 100 == 0) { return 28; }
            else{ return 29; }
        }
        else { return 28; }
        break;
    default:
        return -1;
        break;
    }
}

int main(__attribute__((unused)) int argc, __attribute__((unused)) const char* argv[]) {
    int mois,annee;
    std::cout << "mois" << std::endl;
    std::cin >> mois;
    std::cout << "annee" << std::endl;
    std::cin >> annee;

    std::cout << "nombre de jours dans le mois " << nbjour(mois, annee) << std::endl;
    return 0;
}