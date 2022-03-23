
#include "gpu_meter.h"
#include "logging.h"

int main(int argc, char **argv) {
  GPU_METER->setInterval(5);
  GPU_METER->startMonitor();
  while (true) {
  }
}