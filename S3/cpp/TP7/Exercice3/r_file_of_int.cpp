#include "r_file_of_int.hpp"

R_File_of_Int::R_File_of_Int(const char* name)
    : std::ifstream(name, std::ios::binary)
{

}

R_File_of_Int::~R_File_of_Int() {

}


R_File_of_Int& R_File_of_Int::operator>>(int &value)
{
    char c;
    this->read(&c, 1);
    value = c;
    return *this;
}