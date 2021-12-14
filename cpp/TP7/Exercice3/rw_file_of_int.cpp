#include "rw_file_of_int.hpp"

#include <iostream>

RW_File_of_Int::RW_File_of_Int(const char* name)
    : R_File_of_Int(name),
      W_File_of_Int(name)
{

}

RW_File_of_Int::~RW_File_of_Int()
{

}

void RW_File_of_Int::close()
{
    R_File_of_Int::close();
    W_File_of_Int::close();
}

RW_File_of_Int& RW_File_of_Int::seekp(std::streampos p)
{
    R_File_of_Int::seekg(p);
    W_File_of_Int::seekp(p);

    return *this;
}
RW_File_of_Int& RW_File_of_Int::seekp(std::streamoff off, std::ios_base::seekdir base)
{
    W_File_of_Int::seekp(off, base);
    R_File_of_Int::seekg(off, base);
    return *this;
}

int RW_File_of_Int::tellp()
{
    return W_File_of_Int::tellp();
}


bool RW_File_of_Int::operator!()
{
    return R_File_of_Int::operator!() && W_File_of_Int::operator!();
}

