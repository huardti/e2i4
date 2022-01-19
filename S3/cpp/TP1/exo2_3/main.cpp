#include <vector>

#include "../matrix/matrix.hpp"

int main(__attribute__((unused)) int argc, __attribute__((unused)) const char* argv[]) {
    Matrix<int> m(2,2,0);
    m(0,0) = 0;
    m(1,0) = 1;
    m(0,1) = 2;
    m(1,1) = 3;

    std::cout << "la matrice est :" << std::endl;
    m.print();

    std::cout << "sous forme 1D, elle vaut :" << std::endl;
    std::vector<int> tab = m.tab_1D();
    for (size_t i = 0; i < tab.size(); i++) {
        std::cout << tab[i] << " ";
    }

    m.transpose();

    std::cout << "sa transposée est :" << std::endl;
    m.print();

/* exo 4 */
#if 0
    Matrix<int> m1(3,4,0);
    m1(0,0) = 0;
    m1(1,0) = 1;
    m1(2,0) = 2;
    m1(3,0) = 3;
    m1(0,1) = 4;
    m1(1,1) = 5;
    m1(2,1) = 6;
    m1(3,1) = 7;
    m1(0,2) = 8;
    m1(1,2) = 9;
    m1(2,2) = 10;
    m1(3,2) = 11;

    Matrix<int> m2(4,3,0);
    m2(0,0) = 1;
    m2(1,0) = 1;
    m2(2,0) = 1;
    m2(0,1) = 0;
    m2(1,1) = 0;
    m2(2,1) = 0;
    m2(0,2) = 0;
    m2(1,2) = 0;
    m2(2,2) = 0;
    m2(0,3) = 0;
    m2(1,3) = 0;
    m2(2,3) = 0;

    m1.print();
    m2.print();

    Matrix<int> res = multi(m1, m2);
    res.print();
#endif
    return 0;
}