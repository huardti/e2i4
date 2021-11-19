#pragma once

class Pile
{
public:
  Pile(int max) : _max(max)
  {
    _nb_elem = 0;
    _etat = vide;
    _table = new int[max];

    _nb_pile++;
  }
  ~Pile()
  {
    _nb_pile--;
    delete[] _table;
  }

  void empiler(int);
  int depiler();
  void afficher() const;
  inline int nb_elements() const { return _nb_elem; }

  static inline int nb_pile() { return _nb_pile; }

private:
  const int _max;
  int *_table;
  int _nb_elem;
  enum pile_etat { ok, plein, vide };
  pile_etat _etat;

  static int _nb_pile;
};