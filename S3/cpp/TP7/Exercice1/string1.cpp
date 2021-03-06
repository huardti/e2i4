#include "string1.hpp"

#include <iostream>
#include <cassert>

String::String(const char * c)
{
    m_chaine = new char[std::strlen(c)+1];
    for (size_t i = 0; i < std::strlen(c); i++) {
        m_chaine[i] = c[i];
    }
    m_chaine[strlen(c)] = '\0';
}

String::String(const String& obj)
{
    m_chaine = new char[std::strlen(obj.m_chaine)+1];
    std::strcpy(m_chaine, obj.m_chaine);
}

String::String(const char c, const unsigned int n)
{
    m_chaine = new char[n+1];
    for (size_t i = 0; i < n; i++) {
        m_chaine[i] = c;
    }
    m_chaine[n] = '\0';
}


String::~String()
{
    delete [] m_chaine;
}

const char& String::nieme(const unsigned int n) const
{
    assert( n < (unsigned int)longueur() + 1);
    return m_chaine[n-1];
}

char& String::nieme(const unsigned int n)
{
    assert( n < (unsigned int)longueur() + 1);
    return m_chaine[n-1];
}

void String::affiche()
{
    std::cout << m_chaine;
}

void String::saisie(std::istream& stream)
{
    delete [] m_chaine;
    m_chaine = new char[1024+1];
    stream.getline(m_chaine, 1024);
    m_chaine[1024] = '\0';
}

void String::concatene(const char* ch)
{
    char* new_str = new char[strlen(m_chaine) + strlen(ch) + 1];
    strcpy(new_str, m_chaine);
    delete [] m_chaine;
    m_chaine = new_str;

    strcat(new_str, ch);

}
void String::concatene(const char c)
{
    char s[1];
    s[0] = c;
    concatene(s);
}

bool egal(String a, String b)
{
    if (strcmp(a.m_chaine, b.m_chaine) == 0)
    { return 1; }
    else { return 0; }
}

String String::minuscule()
{
    String r(*this);
    for (size_t i = 0; i < strlen(r.m_chaine); i++) {
        r.m_chaine[i] = tolower((unsigned char) m_chaine[i]);
    }
    return r;
}