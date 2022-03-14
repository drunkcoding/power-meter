#ifndef METER_LOGGING_H
#define METER_LOGGING_H

#include "log_stream.h"
#include "logging.h"

#define DEVICE_LOG_TRACE                                      \
  if (Logger::logLevel() <= Logger::TRACE)                     \
  Logger(__FILE__, __LINE__, Logger::TRACE, __func__).stream() \
      << "[" << deviceID << "] "
#define DEVICE_LOG_DEBUG                                      \
  if (Logger::logLevel() <= Logger::DEBUG)                     \
  Logger(__FILE__, __LINE__, Logger::DEBUG, __func__).stream() \
      << "[" << deviceID << "] "
#define DEVICE_LOG_INFO                                      \
  if (Logger::logLevel() <= Logger::INFO)                     \
  Logger(__FILE__, __LINE__, Logger::INFO, __func__).stream() \
      << "[" << deviceID << "] "
#define DEVICE_LOG_WARN                                      \
  if (Logger::logLevel() <= Logger::WARN)                     \
  Logger(__FILE__, __LINE__, Logger::WARN, __func__).stream() \
      << "[" << deviceID << "] "
#define DEVICE_LOG_ERROR                                      \
  if (Logger::logLevel() <= Logger::ERROR)                     \
  Logger(__FILE__, __LINE__, Logger::ERROR, __func__).stream() \
      << "[" << deviceID << "] "
#define DEVICE_LOG_FATAL                                      \
  if (Logger::logLevel() <= Logger::FATAL)                     \
  Logger(__FILE__, __LINE__, Logger::FATAL, __func__).stream() \
      << "[" << deviceID << "] "

#define METER_LOG_TRACE                      \
  if (Logger::logLevel() <= Logger::TRACE) \
  Logger(__FILE__, __LINE__, Logger::TRACE, __func__).stream()
#define METER_LOG_DEBUG                      \
  if (Logger::logLevel() <= Logger::DEBUG) \
  Logger(__FILE__, __LINE__, Logger::DEBUG, __func__).stream()
#define METER_LOG_INFO                      \
  if (Logger::logLevel() <= Logger::INFO) \
  Logger(__FILE__, __LINE__, Logger::INFO, __func__).stream()
#define METER_LOG_WARN                      \
  if (Logger::logLevel() <= Logger::WARN) \
  Logger(__FILE__, __LINE__, Logger::WARN, __func__).stream()
#define METER_LOG_ERROR                      \
  if (Logger::logLevel() <= Logger::ERROR) \
  Logger(__FILE__, __LINE__, Logger::ERROR, __func__).stream()
#define METER_LOG_FATAL                      \
  if (Logger::logLevel() <= Logger::FATAL) \
  Logger(__FILE__, __LINE__, Logger::FATAL, __func__).stream()

#endif
