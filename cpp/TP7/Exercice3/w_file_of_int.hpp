#pragma once

#include <fstream>

class W_File_of_Int : private std::ofstream
{
public:
    W_File_of_Int(const char* name);
    ~W_File_of_Int();

    W_File_of_Int& operator<<(const int &value);

    using std::ostream::operator!;
    using std::ofstream::tellp;
    using std::ofstream::seekp;
    using std::ofstream::close;

};