#pragma once


#include "r_file_of_int.hpp"
#include "w_file_of_int.hpp"

class RW_File_of_Int : private R_File_of_Int, private W_File_of_Int
{
public:
    RW_File_of_Int(const char* name);
    ~RW_File_of_Int();

    // R_File_of_Int& operator>>(int &value);

    bool operator!();

    void close();

    using W_File_of_Int::operator<<;

    using R_File_of_Int::clear;
    using R_File_of_Int::eof;
    using R_File_of_Int::operator>>;

    int tellp();
    RW_File_of_Int& seekp(std::streampos p);
    RW_File_of_Int& seekp(std::streamoff off, std::ios_base::seekdir base);

protected:

};