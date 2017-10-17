#include "precompiled/precompiled_header.h"
#include "laspack_vector.h"

laspack_vector::laspack_vector ()
{
  m_raw = new Vector;
}



laspack_vector::~laspack_vector ()
{
  if (m_raw)
    V_Destr (m_raw);
  delete m_raw;
}

laspack_vector::laspack_vector (const int size)
{
  m_raw = new Vector;

  V_Constr (m_raw, "Vector", size, InstanceType::Normal, Boolean::True);
  V_SetAllCmp(m_raw, 0.0);
}

Vector *laspack_vector::raw ()
{
  return m_raw;
}

const Vector *laspack_vector::raw () const
{
  return m_raw;
}

laspack_vector::laspack_vector (const std::vector<double> v)
{
  m_raw = new Vector;
  V_Constr (m_raw, "Vector", v.size (), InstanceType::Normal, Boolean::True);
  for (int i = 0; i < isize (v); i++)
    {
      V_SetCmp (m_raw, i + 1, v[i]);
    }
}

double laspack_vector::operator[] (const int i) const
{
  return V_GetCmp (m_raw, i + 1);
}
