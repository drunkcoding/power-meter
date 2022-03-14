#ifndef CPU_METER_H
#define CPU_METER_H

#include <chrono>
#include <thread>

#include "meter.h"
#include "meter_logging.h"
#include "struct_util.h"

class CPUMeter : public Meter {
  CPUMeter() {}
  ~CPUMeter() {}

 public:
  static CPUMeter* Instance();

  void runMonitor(int deviceID);

  void initNumDevice();

 private:
  static CPUMeter* self_;
};

#define CPU_METER CPUMeter::Instance()

#endif /* CPU_METER_H */