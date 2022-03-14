#ifndef STRUCT_UTIL_H
#define STRUCT_UTIL_H

#include <nvml.h>

#include <chrono>
#include <cstdint>
#include <cstdio>

#include "log_stream.h"
#include "types.h"

typedef struct gpuStats {
  gpuStats() : timestamp(NOW()), powerUsage(0) {}

  timePoint_t timestamp;
  // std::uint32_t temperature;
  std::uint32_t powerUsage;
  // std::uint32_t powerLimit;
  nvmlUtilization_t utilization;
  // nvmlMemory_t memory;
  // nvmlPstates_t performanceState;

} gpuStats_t;

// LogStream& operator<<(LogStream& os, const gpuStats& rhs) {
//   char buffer[1024];
//   std::snprintf(
//       buffer, 1024,
//       "temperature %d, powerUsage %d, powerLimit %d, utilization(gpu) %d, "
//       "utilization(memory) %d, memory.used %d, memory.total %d, "
//       "rhs.memory.free %d, performanceState %d",
//       rhs.temperature, rhs.powerUsage, rhs.powerLimit, rhs.utilization.gpu,
//       rhs.utilization.memory, rhs.memory.used, rhs.memory.total,
//       rhs.memory.free, rhs.performanceState);
//   os << buffer;
//   return os;
// }

typedef struct {
  timePoint_t timestamp;
  double DRAMEnergy;
  double Energy;
} cpuStats_t;

#endif /* STRUCT_UTIL_H */