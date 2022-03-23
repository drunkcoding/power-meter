#include "gpu_meter.h"

#include <cuda_runtime.h>

#include <chrono>
#include <fstream>

GPUMeter *GPUMeter::self_ = new GPUMeter();

GPUMeter *GPUMeter::Instance() { return self_; }

void GPUMeter::runMonitor(int deviceID) {
  nvmlDevice_t device;
  NVML_RT_CALL(nvmlDeviceGetHandleByIndex(deviceID, &device));

  gpuStats_t stats_before;
  gpuStats_t stats_after;
  gpuStats_t stats;

  while (loop_) {
    auto queryStart = NOW();
    stats.timestamp = queryStart;
    // NVML_RT_CALL(nvmlDeviceGetTemperature(device, NVML_TEMPERATURE_GPU,
    //                                       &stats.temperature));
    NVML_RT_CALL(nvmlDeviceGetPowerUsage(device, &stats.powerUsage));
    // NVML_RT_CALL(
    //     nvmlDeviceGetEnforcedPowerLimit(device, &stats.powerLimit));
    NVML_RT_CALL(nvmlDeviceGetUtilizationRates(device, &stats.utilization));
    // NVML_RT_CALL(nvmlDeviceGetMemoryInfo(device, &stats.memory));
    // NVML_RT_CALL(nvmlDeviceGetPerformanceState(
    //     device, &stats.performanceState));

    // if (stats_before.powerUsage == 0) {
    //   // take first record and continue
    //   std::memcpy(&stats_before, &stats, sizeof(gpuStats_t));
    //   continue;
    // }

    std::memcpy(&stats_before, &stats_after, sizeof(gpuStats_t));
    std::memcpy(&stats_after, &stats, sizeof(gpuStats_t));

    auto period = MICROSECONDS(stats_after.timestamp - stats_before.timestamp);
    // milliWatts
    double average_power =
        (stats_after.powerUsage + stats_before.powerUsage) / 2 / 1000;
    double jourel = average_power * period.count() / 1e6;

    METER_LOG_INFO << "(" << queryStart.time_since_epoch().count() << ")" << " GPU " << deviceID << " Energy(J) " << jourel
                   << " Power(mW) " << stats_after.powerUsage;
    METER_LOG_DEBUG << "Duration (us) " << period.count();
    METER_LOG_DEBUG << "Last power (mW) " << stats_before.powerUsage
                    << ", After power (mW) " << stats_after.powerUsage;
    auto queryEnd = NOW();

    std::this_thread::sleep_for(
        MICROSECONDS(std::chrono::milliseconds(interval_) -
                     MILLISECONDS(queryEnd - queryStart)));
  }
}

void GPUMeter::initNumDevice() {
  int numDevice;
  cudaGetDeviceCount(&numDevice);
  numDevice_ = implicit_cast<std::uint32_t>(numDevice);
}