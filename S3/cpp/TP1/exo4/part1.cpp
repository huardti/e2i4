#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

int main(__attribute__((unused)) int argc, __attribute__((unused)) const char* argv[])
{
    std::vector<std::string> tab;
    std::cout << "vos phrases :" << std::endl;

    std::string tmp;
    for (int i = 0; i < 10; i++) {
        std::cin >> tmp;
        tab.push_back(tmp);
    }
    std::reverse(tab.begin(), tab.end());
    std::cout << "inverse :" << std::endl;
    for (auto elem : tab) {
        std::cout << elem << std::endl;
    }

    return 0;
}