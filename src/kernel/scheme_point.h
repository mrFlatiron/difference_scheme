#ifndef SCHEME_POINT_H
#define SCHEME_POINT_H

#include "scheme_defs.h"

class scheme_point
{
private:
  int m_n;
  int m_m;

  using self = scheme_point;
public:
  scheme_point ();
  ~scheme_point ();

  scheme_point (const int n, const int m);

  int n () const;
  int m () const;

  self &inc (const variable var);
  self &dec (const variable var);
};

#endif // SCHEME_POINT_H
