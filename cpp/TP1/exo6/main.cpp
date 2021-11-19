#include <iostream>


long somme_tab(int tab[], size_t n)
{
    long somme = 0;
    for (size_t i = 0; i < n; i++) {
        somme += tab[i];
    }
    return somme;
}

int main(__attribute__((unused)) int argc, __attribute__((unused)) const char* argv[])
{
    int *tab;
    size_t n;

    do{
        std::cout << "taille du tableau : " << std::endl;
        std::cin >>n;
    } while(n > 100 || n <= 0);

    tab = new int[n];

    std::cout << "saisi des elements :" << std::endl;
    for (size_t i = 0; i < n; i++) {
        std::cin >> tab[i];
    }

    std::cout << "la somme est : " << somme_tab(tab, n) << std::endl;

    std::cout << "tab = ";
    for (size_t i = 0; i < n; i++) {
        std::cout << tab[i] << " ";
    }
    std::cout << std::endl;

    delete tab;
    return 0;
}