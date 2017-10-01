#ifndef MY_UTILS_H
#define MY_UTILS_H

template<typename T>
int isize (const T &container) {return static_cast<int> (container.size ());}

template<typename T, typename Iter = typename T::const_iterator>
Iter u_last (const T &container)
{
  if (!container.size ())
    return container.end ();
  auto it = container.end ();
  it--;
  return it;
}

//#if __cplusplus < 201402L
template<typename T, typename... Args>
std::unique_ptr<T> make_unique (Args&&... args)
{
  return std::unique_ptr<T> (new T (std::forward<Args>(args)...));
}

//#endif // __cplusplus

template<typename T, typename... Args>
void put_in (std::unique_ptr<T> &something, Args&&... args)
{
  something = make_unique<T> (std::forward<Args> (args)...);
}

namespace utils
{

}

#endif // MY_UTILS_H
