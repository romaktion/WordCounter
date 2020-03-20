#pragma once

#ifdef __linux__
#include <sys/time.h>
#elif _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock.h>
#else
#endif


class timing
{
public:
  timing();
  ~timing();

  void start();
  double get();

private:
  timeval timeval_start;
#ifdef _WIN32
  int gettimeofday(struct timeval* tp, struct timezone* tzp);
#endif
};
