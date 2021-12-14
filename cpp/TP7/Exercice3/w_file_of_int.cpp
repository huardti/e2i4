#include "w_file_of_int.hpp"

W_File_of_Int::W_File_of_Int(const char* name)
    : std::ofstream(name, std::ios::binary)
{

}

W_File_of_Int::~W_File_of_Int()
{

}


W_File_of_Int& W_File_of_Int::operator<<(const int &value)
{
    char c = static_cast<char>(value);
    this->write(&c, 1);
    return *this;
}