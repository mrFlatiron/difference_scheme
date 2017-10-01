#ifndef SIMPLE_VECTOR_H
#define SIMPLE_VECTOR_H


class simple_vector
{
private:
  double *m_data = nullptr;
  int m_size = 0;
  bool m_is_owner = true;
public:
  simple_vector ();
 ~simple_vector ();

  simple_vector (double *data, const int size, const bool need_copy = false);
  explicit simple_vector (const int size);
  simple_vector (const simple_vector &vect);
  simple_vector (simple_vector &&vect);

  simple_vector &operator= (const simple_vector &vector);
  simple_vector &operator= (simple_vector &&vect);

  simple_vector (const std::vector<double> &stdvec);

  int size () const;
  double &operator[] (const int i);
  const double &operator[] (const int i) const;
  double at (const int i) const;
  double *data ();
  const double *data () const;
  void resize (const int size);
};

#endif // SIMPLE_VECTOR_H
