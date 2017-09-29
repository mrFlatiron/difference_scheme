#include "scheme_point.h"

scheme_point::scheme_point ()
{

}

scheme_point::~scheme_point ()
{

}

scheme_point::scheme_point(const int n, const int m)
  : m_n (n),
    m_m (m)
{

}

int scheme_point::n () const
{
  return m_n;
}

int scheme_point::m () const
{
  return m_m;
}

scheme_point::self &scheme_point::inc (const variable var)
{
  switch (var)
    {
    case variable::t:
      m_n++;
      return *this;
    case variable::x:
      m_m++;
      return *this;
    }
  DEBUG_PAUSE ("Shouldn't happen");
  return *this;
}

scheme_point::self &scheme_point::dec (const variable var)
{
  switch (var)
    {
    case variable::t:
      m_n--;
      return *this;
    case variable::x:
      m_m--;
      return *this;
    }
  DEBUG_PAUSE ("Shouldn't happen");
  return *this;
}
