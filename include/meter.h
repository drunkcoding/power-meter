#ifndef METER_H
#define METER_H

#include <chrono>
#include <cstdint>
#include <thread>
#include <vector>

#include "noncopyable.h"
#include "types.h"

class Meter : noncopyable {

 public:
  void setInterval(std::uint32_t interval) { interval_ = interval; }
  void setNumDevice(std::uint32_t numDevice) { numDevice_ = numDevice; }
  
  virtual void initNumDevice() = 0;

  // void runMonitor(int deviceID) {
  //   while (loop_) {
  //     auto queryStart = NOW();

  //     auto queryEnd = NOW();
  //     auto queryDuration =
  //         std::chrono::duration_cast<std::chrono::milliseconds>(queryEnd -
  //                                                               queryStart);
  //     if (queryDuration.count() < interval_) {
  //       std::this_thread::sleep_for(
  //           std::chrono::duration_cast<std::chrono::microseconds>(
  //               std::chrono::milliseconds(interval_) - queryDuration));
  //     }
  //   }
  // }
  void startMonitor() {
    loop_ = true;

    initNumDevice();

    for (std::uint32_t deviceID = 0; deviceID < numDevice_; deviceID++) {
      std::thread monitor(&Meter::runMonitor, this, deviceID);
      monitor.detach();
      // threads_.push_back(std::move(monitor));
      // std::cout << "threads_" << deviceID << std::endl;
    }
  }

  // virtual void startMonitor() = 0;
  virtual void runMonitor(int deviceID) = 0;

  void stopMonitor() { loop_ = false; }

 protected:
  bool loop_ = false;
  std::uint32_t interval_ = 10;
  std::uint32_t numDevice_ = 0;
  void (*monitorPtr)(){nullptr};
  std::vector<std::thread> threads_;
};

#endif