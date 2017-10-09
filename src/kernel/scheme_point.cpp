#include "scheme_point.h"

scheme_point::scheme_point ()
{

}

scheme_point::~scheme_point ()
{

}

scheme_point::scheme_point (const int n, const int m)
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

scheme_point::self scheme_point::inc (const variable var)
{
  self retval = *this;
  switch (var)
    {
    case variable::t:
      retval.m_n++;
      return retval;
    case variable::x:
      retval.m_m++;
      return retval;
    }
  DEBUG_PAUSE ("Shouldn't happen");
  return retval;
}

scheme_point::self scheme_point::dec (const variable var)
{
  self retval = *this;
  switch (var)
    {
    case variable::t:
      retval.m_n--;
      return retval;
    case variable::x:
      retval.m_m--;
      return retval;
    }
  DEBUG_PAUSE ("Shouldn't happen");
  return retval;
}
