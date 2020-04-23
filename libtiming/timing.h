#pragma once

#ifdef __linux__
#include <sys/time.h>
#elif _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock.h>
#else
#endif

#ifdef libtiming_EXPORTS
#define libtiming_API __declspec(dllexport)
#else
#define libtiming_API __declspec(dllimport)
#endif

class
#ifdef _WIN32
  libtiming_API
#endif
  timing
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
