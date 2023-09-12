#ifndef __PROJECTS_CPP_SERVER_V2_SRC_BASE_CONCURRENCY_BLOCKINGQUEUE_H_
#define __PROJECTS_CPP_SERVER_V2_SRC_BASE_CONCURRENCY_BLOCKINGQUEUE_H_

#include <condition_variable>
#include <mutex>
#include <queue>
#include <vector>

namespace CppServer {
namespace base {
template <typename ValueType> class BlockingQueue {
public:
  using Iterator = ValueType *;
  using ConstIterator = const ValueType *;

private:
  std::queue<ValueType> data;
  std::mutex m;
  std::condition_variable cv;

public:
  void push(ValueType &value) {
    std::lock_guard<std::mutex> lk(m);
    data.push(value);
    cv.notify_one();
  }

  void push(ValueType &&value) {
    std::lock_guard<std::mutex> lk(m);
    data.push(std::move(value));
    cv.notify_one();
  }

  ValueType pop() {
    std::unique_lock<std::mutex> lk(m);
    cv.wait(lk, [this]() { return !data.empty(); });
    ValueType val = data.front();
    data.pop();
    return val;
  }

  void swap(BlockingQueue &other) { std::swap(data, other.data); }

  std::vector<ValueType> popAll() {
    std::unique_lock<std::mutex> lk(m);
    std::vector<ValueType> res;
    while (!data.empty()) {
      res.push_back(std::move(data.front()));
      data.pop();
    }
    return res;
  }
};
} // namespace base
} // namespace CppServer

#endif // __PROJECTS_CPP_SERVER_V2_SRC_BASE_CONCURRENCY_BLOCKINGQUEUE_H_