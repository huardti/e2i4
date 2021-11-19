#include "vect.hpp"

int main(int argc, const char* argv[]) {
    Vect_3d<double> test{1,2,3};
    Vect_3d<double> ouch;
    const Vect_3d<double> test2{test};
    test.print();
    ouch.print();
    test2.print();

    return 0;
}