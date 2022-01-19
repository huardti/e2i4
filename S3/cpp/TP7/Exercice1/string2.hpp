#pragma once

#include "string1.hpp"

#include <sstream>
class String2 : public String
{
public:
    using String::String;

    String2 minuscule() const ;

    String2& operator=(const String2 &a);

    bool operator==(const String2& a) const;
    bool operator!=(const String2& a) const;

    String2& operator+=(const char a);
    String2 operator+(const char a) const;

    String2& operator+=(const char* a);
    String2 operator+(const char* a) const;

    friend String2 operator+(const char* c, const String2 s);

    char& operator[](const unsigned int idx) { return nieme(idx); };
    const char& operator[](const unsigned int idx) const  { return nieme(idx); };

    friend std::ostream& operator<<(std::ostream& stream, const String2& s);
    friend std::istream& operator>>(std::istream& stream, String2& s);
};