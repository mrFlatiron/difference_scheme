#ifndef SCHEME_DEFS
#define SCHEME_DEFS


enum class solver_state
{
  invalid,
  ready,
  in_progress,
  solved,
  failed
};
enum class deriv_type
{
  wide,
  fw, //forward
  bw  //backward
};
enum class variable
{
  t,
  x
};
enum class grid_func
{
  G,
  V
};

#endif // SCHEME_DEFS

