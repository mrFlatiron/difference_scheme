#ifndef DEBUG_UTILS_H
#define DEBUG_UTILS_H

#ifdef __GNUC__

  #include <csignal>
  #define DEBUG_PAUSE(reason) (std::raise (SIGTRAP));

#else

  #ifdef __WIN32__

    #define DEBUG_PAUSE(reason) (__debugbreak ());

  #else

    #define DEBUG_PAUSE(reason)

  #endif //__WIN32__

#endif //__GNUC>__

#endif // DEBUG_UTILS_H
