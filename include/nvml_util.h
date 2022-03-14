
#ifndef NVML_UTIL_H
#define NVML_UTIL_H

#include <nvml.h>

#include <cstdio>

// *************** FOR ERROR CHECKING *******************
#ifndef NVML_RT_CALL
#define NVML_RT_CALL(call)                                                 \
  {                                                                        \
    auto status = static_cast<nvmlReturn_t>(call);                         \
    if (status != NVML_SUCCESS)                                            \
      fprintf(stderr,                                                      \
              "ERROR: CUDA NVML call \"%s\" in line %d of file %s failed " \
              "with "                                                      \
              "%s (%d).\n",                                                \
              #call, __LINE__, __FILE__, nvmlErrorString(status), status); \
  }
#endif  // NVML_RT_CALL
// *************** FOR ERROR CHECKING *******************

#endif /* NVML_UTIL_H */