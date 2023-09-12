#ifndef __PROJECTS_CPP_SERVER_V2_SRC_BASE_CONCURRENCY_SPSCQUEUE_H_
#define __PROJECTS_CPP_SERVER_V2_SRC_BASE_CONCURRENCY_SPSCQUEUE_H_

#include <atomic>
#include <iostream>
#include <vector>
namespace CppServer {
namespace base {
template <typename ValueType> class SPSCQueue {
public:
  static inline constexpr int QueueSize = 1e5;

private:
  std::vector<ValueType> data_;
  std::atomic<int> head_;
  std::atomic<int> tail_;

  int inc(int idx) { return (idx + 1) % QueueSize; }

  bool internalPush(ValueType &&value) {
    // as long as it is not full
    int lastHead = head_.load();
    if (lastHead == inc(tail_)) {
      return false;
    }
    data_[tail_] = std::move(value);
    tail_.store(inc(tail_));
    return true;
  }

  ValueType *internalPop() {
    int lastTail = tail_.load();
    if (head_ == lastTail) {
      return nullptr;
    }
    ValueType *res = &data_[head_];
    head_.store(inc(head_));
    return res;
  }

public:
  SPSCQueue() : data_(QueueSize), head_(0), tail_(0) {}

  void push(ValueType &value) { push(std::move(value)); }

  void push(ValueType &&value) {
    while (!internalPush(std::move(value)))
      ;
  }

  ValueType pop() {
    ValueType *resAddr = nullptr;
    while (!(resAddr = internalPop()))
      ;
    return std::move(*resAddr);
  }
};
} // namespace base
} // namespace CppServer

#endif // __PROJECTS_CPP_SERVER_V2_SRC_BASE_CONCURRENCY_SPSCQUEUE_H_
