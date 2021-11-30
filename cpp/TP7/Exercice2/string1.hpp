#pragma once

#include <cstring>
// #include <sstream>
#include <iostream>
class String
{
public:
    String(const char * c);
    String (const char c, const unsigned int i);
    String(const String& obj);

    ~String();

    inline int longueur() const { return std::strlen(m_chaine); }
    char& nieme(const unsigned int n);
    const char& nieme(const unsigned int n) const;
    void affiche();

    void saisie(std::istream& stream = std::cin);
    void concatene(const char* ch);
    void concatene(const char c);
    friend bool egal(String a, String b);
    String minuscule();

protected:
    char * m_chaine;
};