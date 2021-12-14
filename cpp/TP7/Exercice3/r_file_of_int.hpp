#pragma once

#include <fstream>

class R_File_of_Int : private std::ifstream
{
  public:
    R_File_of_Int(const char* name);
    ~R_File_of_Int();

    R_File_of_Int& operator>>(int &value);

    using std::istream::operator!;
    using std::ifstream::seekg;
    using std::ifstream::tellg;
    using std::ifstream::clear;
    using std::ifstream::close;
    using std::ifstream::eof;
  protected:

};