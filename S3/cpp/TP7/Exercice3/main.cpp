#include <iostream>

#include "r_file_of_int.hpp"
#include "w_file_of_int.hpp"

#include "rw_file_of_int.hpp"

int main() {
    W_File_of_Int f_out("essai.fic");
    if (!f_out) {
        std::cerr << "erreur à la création de 'essai.fic'\n";
        return 1;
    }
    for (int i = 0; i <= 10; i++) // Ecriture de 11 entiers dans le fichier
        f_out << i;
    std::cout << f_out.tellp() << " éléments sont écrits dans le fichier.\n";
    // affiche: 11 éléments sont écrits dans le fichier.
    f_out.close();
    R_File_of_Int f_in("essai.fic");
    int entier;
    if (!f_in) {
        std::cerr << "erreur à la création de essai.fic\n";
        return 1;
    }
    f_in.seekg(0, std::ios::end); // se positionne à la fin du fichier
    std::cout << "Il y a " << f_in.tellg() << " éléments dans le fichier.\n";
    // affiche: Il y a 11 éléments dans le fichier.
    f_in.seekg(0,std::ios::beg);
    // se positionne au début du fichier
    while (1) {
        // affichage du contenu du fichier
        f_in >> entier;
        if (f_in.eof()){
            break;
        }
        std::cout << entier << " ";
    }
    f_in.clear(); // ne pas l'oublier ... sortie du while sur erreur eof
    std::cout << std::endl;
    f_in.close();
    /////////////////////////////////////////////////////////////////
    RW_File_of_Int f_io("essai.fic"); // s’il existe, il n'est pas écrasé
    if (!f_io) {
        std::cerr << "erreur à la création de essai.fic\n";
        return 1;
    }
    f_io.seekp(0, std::ios::end);
    // se positionne à la fin du fichier
    std::cout << "Il ya déjà " << f_io.tellp() << " éléments dans le fichier\n";
    // affiche: Il y a déjà 11 éléments dans le fichier
    for (int i = 11; i <= 19; i++)
    {
        f_io << i;
    }

    f_io.seekp(0);
    // se positionne sur le 11 ième entier
    while (1) {
        // affichage du contenu du fichier
        f_io >> entier;
        if (f_io.eof()){
            break;
        }
        std::cout << entier << " ";
    }             // affiche: 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19
    f_io.clear(); // ne pas l'oublier... sortie du while sur erreur eof
    f_io.close();

    return 0;
}