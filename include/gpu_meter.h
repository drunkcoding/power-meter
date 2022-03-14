#ifndef GPU_METER_H
#define GPU_METER_H

#include <chrono>
#include <thread>

#include "meter.h"
#include "meter_logging.h"
#include "nvml_util.h"
#include "struct_util.h"

class GPUMeter : public Meter {
 private:
  GPUMeter() { NVML_RT_CALL(nvmlInit()); }
  ~GPUMeter() { NVML_RT_CALL(nvmlShutdown()); }

 public:
  static GPUMeter *Instance();

  void runMonitor(int deviceID);

  void initNumDevice();

  // void startMonitor() {
  //   int numDevice;
  //   // Assuming the device are indexed from zero
  //   // Monitoring all devices
  //   cudaGetDeviceCount(&numDevice);

  //   loop_ = true;

  //   for (int deviceID = 0; deviceID < numDevice; deviceID++) {
  //     std::thread monitor(&GPUMeter::runMonitor, this, deviceID);
  //     monitor.detach();
  //   }
  // }

 private:
  static GPUMeter *self_;
};

#define GPU_METER GPUMeter::Instance()

#endif /* GPU_METER_H */