#ifndef __PROJECTS_CPP_SERVER_V2_SRC_CORE_EVENTLOOPPOOL_H_
#define __PROJECTS_CPP_SERVER_V2_SRC_CORE_EVENTLOOPPOOL_H_

#include "core/EventLoop.h"
namespace CppServer {
namespace core {

template <size_t NumOfLoop> class EventLoopPool {
private:
  std::vector<EventLoop> pool_ = std::vector<EventLoop>(NumOfLoop);
  int poolIdx_ = 0;

  int inc() {
    int curIdx = poolIdx_;
    poolIdx_ = (poolIdx_ + 1) % NumOfLoop;
    return curIdx;
  }

public:
  EventLoopPool() {
    for (auto &loop : pool_) {
      loop.loop();
    }
  }

  ~EventLoopPool() {
    for (auto &loop : pool_) {
      loop.join();
    }
  }

  EventLoop &getNextLoop() { return pool_[inc()]; }
};
} // namespace core
} // namespace CppServer

#endif // __PROJECTS_CPP_SERVER_V2_SRC_CORE_EVENTLOOPPOOL_H_