#ifndef __PROJECTS_CPP_SERVER_V2_SRC_BASE_CONCURRENCY_THREADPOOL_H_
#define __PROJECTS_CPP_SERVER_V2_SRC_BASE_CONCURRENCY_THREADPOOL_H_

#include <atomic>
#include <functional>
#include <future>
#include <memory>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>
namespace CppServer {
namespace base {
template <template <class> class QueueStructure, int NumThread>
class ThreadPool {
public:
  using TaskFunc = std::function<void()>;

private:
  QueueStructure<TaskFunc> queue;
  std::vector<std::thread> threads;
  std::atomic_bool terminate = false;

  void workerLoop() {
    while (!terminate) {
      TaskFunc task = queue.pop();
      task();
    }
  }

  void teardown() {
    terminate = true;
    queue.push([this]() { teardown(); });
  }

public:
  ThreadPool() {
    for (int i = 0; i < NumThread; ++i) {
      threads.emplace_back(&ThreadPool::workerLoop, this);
    }
  }

  ~ThreadPool() {
    teardown();
    join();
  }

  template <typename Func, typename... Args>
  std::future<std::result_of_t<Func(Args...)>> addTask(Func &&func,
                                                       Args &&...args) {
    using ResultType = std::result_of_t<Func(Args...)>;

    auto task = std::make_shared<std::packaged_task<ResultType()>>(
        std::bind(std::forward<Func>(func), std::forward<Args>(args)...));

    std::future<ResultType> res = task->get_future();
    queue.push([task]() { (*task)(); });
    return res;
  }

  void join() {
    for (auto &t : threads) {
      t.join();
    }
  }
};
} // namespace base
} // namespace CppServer

#endif // __PROJECTS_CPP_SERVER_V2_SRC_BASE_CONCURRENCY_THREADPOOL_H_