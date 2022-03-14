#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <chrono>
#include <map>
#include <sstream>
#include <thread>

#include "logging.h"
#include "types.h"

__thread char t_errnobuf[512];
// __thread char t_time[32];
// __thread time_t t_lastSecond;

std::map<std::string, Logger::LogLevel> log_level_map = {
    {"trace", Logger::TRACE}, {"debug", Logger::DEBUG},
    {"info", Logger::INFO},   {"warning", Logger::WARN},
    {"error", Logger::ERROR}, {"fatal", Logger::FATAL}};

const char* strerror_tl(int savedErrno) {
  return strerror_r(savedErrno, t_errnobuf, sizeof t_errnobuf);
}

Logger::LogLevel initLogLevel() {
  if (::getenv("MUDUO_LOG_TRACE"))
    return Logger::TRACE;
  else if (::getenv("MUDUO_LOG_DEBUG"))
    return Logger::DEBUG;
  else
    return Logger::INFO;
}

Logger::LogLevel g_logLevel = initLogLevel();

const char* LogLevelName[Logger::NUM_LOG_LEVELS] = {
    "TRACE ", "DEBUG ", "INFO  ", "WARN  ", "ERROR ", "FATAL ",
};

// helper class for known string length at compile time
class T {
 public:
  T(const char* str, unsigned len) : str_(str), len_(len) {
    assert(strlen(str) == len_);
  }

  const char* str_;
  const unsigned len_;
};

inline LogStream& operator<<(LogStream& s, T v) {
  s.append(v.str_, v.len_);
  return s;
}

inline LogStream& operator<<(LogStream& s, const Logger::SourceFile& v) {
  s.append(v.data_, v.size_);
  return s;
}

void defaultOutput(const char* msg, int len) {
  size_t n = fwrite(msg, 1, len, stdout);
  // FIXME check n
  (void)n;
}

void defaultFlush() { fflush(stdout); }

Logger::OutputFunc g_output = defaultOutput;
Logger::FlushFunc g_flush = defaultFlush;

Logger::Impl::Impl(LogLevel level, int savedErrno, const SourceFile& file,
                   int line)
    : time_(NOW()), stream_(), level_(level), line_(line), basename_(file) {
  formatTime();
  
  std::ostringstream ss;
  ss << std::this_thread::get_id();
  std::string idstr = ss.str();

  stream_ << '[' << ss.str() << ']';
  stream_ << '[' << T(LogLevelName[level], 6) << ']';
  if (savedErrno != 0) {
    stream_ << strerror_tl(savedErrno) << " (errno=" << savedErrno << ") ";
  }
}

void Logger::Impl::formatTime() {
  // const auto now = NOW();
  // const auto nowAsTimeT = std::chrono::system_clock::to_time_t(now);
  // const auto nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(
  //                        now.time_since_epoch()) %
  //                    1000;
  // std::stringstream nowSs;
  // nowSs << std::put_time(std::localtime(&nowAsTimeT), "%a %b %d %Y %T") << '.'
  //       << std::setfill('0') << std::setw(3) << nowMs.count();

  // stream_ << nowSs.str();
}

void Logger::Impl::finish() {
  stream_ << " - " << basename_ << ':' << line_ << '\n';
}

Logger::Logger(SourceFile file, int line) : impl_(INFO, 0, file, line) {}

Logger::Logger(SourceFile file, int line, LogLevel level, const char* func)
    : impl_(level, 0, file, line) {
  impl_.stream_ << '[' << func << "] ";
}

Logger::Logger(SourceFile file, int line, LogLevel level)
    : impl_(level, 0, file, line) {}

Logger::Logger(SourceFile file, int line, bool toAbort)
    : impl_(toAbort ? FATAL : ERROR, errno, file, line) {}

Logger::~Logger() {
  impl_.finish();
  const LogStream::Buffer& buf(stream().buffer());
  g_output(buf.data(), buf.length());
  if (impl_.level_ == FATAL) {
    g_flush();
    abort();
  }
}

void Logger::setLogLevel(Logger::LogLevel level) { g_logLevel = level; }
void Logger::setLogLevel(const std::string& level) {
  if (log_level_map.find(level) != log_level_map.end()) {
    g_logLevel = log_level_map[level];
  }
}

void Logger::setOutput(OutputFunc out) { g_output = out; }

void Logger::setFlush(FlushFunc flush) { g_flush = flush; }
