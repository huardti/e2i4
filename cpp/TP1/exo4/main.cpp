#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <array>

int main(__attribute__((unused)) int argc, __attribute__((unused)) const char* argv[])
{
    std::vector<std::string> mots;
    std::cout << "vos mots :" << std::endl;

    std::string tmp;
    for (int i = 0; i < 10; i++) {
        std::cin >> tmp;
        mots.push_back(tmp);
    }
    std::sort(mots.begin(), mots.end());
    std::cout << "les mots dans l'orde alphabetiques sont : " << std::endl;
    for (auto elem : mots) {
        std::cout << elem << std::endl;
    }

    while (!mots.empty()) {
        char tmp[10];
        std::cout << "supprimer le premier mot " << std::endl;
        std::cin >> tmp;
        mots.erase(mots.begin());
        std::cout << "mots restant" << std::endl;
        for (auto elem : mots) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}