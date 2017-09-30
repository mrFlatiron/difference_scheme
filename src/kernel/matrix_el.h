#ifndef MATRIX_EL_H
#define MATRIX_EL_H


class matrix_el
{
private:
  int m_i;
  int m_j;
  double m_val;
public:
  matrix_el ();
  ~matrix_el ();

  matrix_el (const int i, const int j, const double val);

  int i () const;
  int j () const;
  double val () const;

  bool operator < (const matrix_el &rhs) const;
};

#endif // MATRIX_EL_H
