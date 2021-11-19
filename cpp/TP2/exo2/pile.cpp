#include "pile.hpp"

#include <iostream>

void Pile::empiler(int element)
{
  if (_etat != plein)
    _table[_nb_elem++] = element;
  else
    std::cout << "pile pleine" << std::endl;
  if (_nb_elem >= _max)
    _etat = plein;
  else
    _etat = ok;
}

int Pile::depiler()
{
  int element = 0;
  if (_etat != vide)
    element = _table[--_nb_elem];
  else
    std::cout << "pile vide" << std::endl;
  if (_nb_elem <= 0)
    _etat = vide;
  else
    _etat = ok;
  return element;
}

void Pile::afficher() const
{
  for (int i = _nb_elem - 1; i >= 0; i--)
    std::cout << _table[i] << std::endl;
}

int Pile::_nb_pile = 0;