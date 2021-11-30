#include "string2.hpp"

#include <iostream>

String2 String2::minuscule() const
{
    String2 r(*this);
    for (size_t i = 0; i < strlen(r.m_chaine); i++) {
        r.m_chaine[i] = tolower((unsigned char) m_chaine[i]);
    }
    return r;
}

String2 operator+(const char* c,const String2 s)
{
    String2 tmp(c);
    tmp.concatene(s.m_chaine);
    return tmp;
}

String2& String2::operator+=(const char a)
{
    char tmp[1];
    tmp[0] = a;
    return *this += tmp;
}

String2 String2::operator+(const char a) const
{
    String2 r(*this);
    return r+= a;
}

String2& String2::operator+=(const char *a)
{
    this->concatene(a);
    return *this;
}

String2 String2::operator+(const char* a) const
{
    String2 r(*this);
    return r += a;
}

bool String2::operator==(const String2& a) const
{
    return egal(*this, a);
}

bool String2::operator!=(const String2& a) const
{
    return !((*this) == a);
}

String2& String2::operator=(const String2 &a)
{
    delete []m_chaine;
    m_chaine = new char[std::strlen(a.m_chaine)];
    std::memcpy(m_chaine, a.m_chaine, sizeof(char)*std::strlen(a.m_chaine));
    return *this;
}

std::ostream& operator<<(std::ostream& stream, const String2& s)
{
    stream << s.m_chaine;
    return stream;
}

std::istream& operator>>(std::istream& stream, String2& s)
{
    s.saisie(stream);
    return stream;
}