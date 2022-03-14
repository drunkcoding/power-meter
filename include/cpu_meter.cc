#include "cpu_meter.h"

#include <chrono>

#include "cpucounters.h"

using namespace pcm;

void CPUMeter::runMonitor(int deviceID) {
  ServerUncoreCounterState beforeState, afterState;

  PCM* m = PCM::getInstance();
  std::int64_t beforeTime = 0, afterTime = 0;

  beforeState = m->getServerUncoreCounterState(deviceID);
  beforeTime = m->getTickCount();

  while (loop_) {
    auto queryStart = NOW();
    // stats.timestamp = queryStart;

    afterState = m->getServerUncoreCounterState(deviceID);
    afterTime = m->getTickCount();

    // 1000. * getDRAMConsumedJoules(beforeState, afterState) /
    // double(afterTime - beforeTime) getDRAMConsumedJoules(beforeState,
    // afterState)

    // 1000. * getConsumedJoules(beforeState, afterState) / double(afterTime -
    // beforeTime) getConsumedJoules(beforeState, afterState)

    METER_LOG_INFO << "CPU Energy (J) "
                   << getConsumedJoules(beforeState, afterState) +
                          getDRAMConsumedJoules(beforeState, afterState);

    auto queryEnd = NOW();

    std::this_thread::sleep_for(
        std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::milliseconds(10) - (queryEnd - queryStart)));
  }
}

void CPUMeter::initNumDevice() {
  PCM* m = PCM::getInstance();
  auto numSockets = m->getNumSockets();
  numDevice_ = implicit_cast<std::uint32_t>(numSockets);
}