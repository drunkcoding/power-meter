#ifndef BLOCKING_QUEUE_H
#define BLOCKING_QUEUE_H

#include <cassert>
#include <condition_variable>
#include <deque>
#include <mutex>

template <typename T>
class BlockingQueue : noncopyable {
 public:
  BlockingQueue() : mutex_(), notEmpty_(mutex_), queue_() {}

  void put(const T& x) {
    std::lock_guard(mutex_);
    queue_.push_back(x);
    notEmpty_.notify();  // wait morphing saves us
    // http://www.domaigne.com/blog/computing/condvars-signal-with-mutex-locked-or-not/
  }

  void put(T&& x) {
    std::lock_guard(mutex_);
    queue_.push_back(std::move(x));
    notEmpty_.notify();
  }

  T take() {
    std::lock_guard(mutex_);
    // always use a while-loop, due to spurious wakeup
    while (queue_.empty()) {
      notEmpty_.wait();
    }
    assert(!queue_.empty());
    T front(std::move(queue_.front()));
    queue_.pop_front();
    return std::move(front);
  }

  size_t size() const {
    std::lock_guard(mutex_);
    return queue_.size();
  }

 private:
  mutable std::mutex mutex_;
  std::condition_variable notEmpty_;
  std::deque<T> queue_;
};

#endif /* BLOCKING_QUEUE_H */
