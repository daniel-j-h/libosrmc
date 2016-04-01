#ifndef OSRMC_VISIBILITY_H_
#define OSRMC_VISIBILITY_H_

#if __GNUC__ >= 4
  #define OSRMC_API __attribute__((visibility("default")))
#else
  #define OSRMC_API
#endif

#endif
