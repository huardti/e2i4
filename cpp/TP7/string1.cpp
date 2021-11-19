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
    m_chaine = new char[std::strlen(obj.m_chaine)];
    for (size_t i = 0; i < std::strlen(obj.m_chaine); i++) {
        m_chaine[i] = obj.m_chaine[i];
    }
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

void String::saisie()
{
    delete [] m_chaine;
    m_chaine = new char[1024+1];
    std::cin.getline(m_chaine, 1024);
    m_chaine[1024] = '\0';
}

void String::concatene(const char* ch)
{
    char* new_str = new char[strlen(m_chaine) + strlen(ch)];
    strcpy(new_str, m_chaine);
    strcat(new_str, ch);
    delete [] m_chaine;
    m_chaine = new_str;

}
void String::concatene(const char c)
{
    char s[1];
    s[0] = c;
    concatene(s);
}

bool egal()
{

}
