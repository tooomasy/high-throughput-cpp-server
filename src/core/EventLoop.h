#ifndef __PROJECTS_CPP_SERVER_V2_SRC_CORE_EVENTLOOP_H_
#define __PROJECTS_CPP_SERVER_V2_SRC_CORE_EVENTLOOP_H_

#include "base/concurrency/BlockingQueue.h"
#include "base/concurrency/SPSCQueue.h"
#include "base/concurrency/ThreadPool.h"
#include "core/Epoller.h"
#include "core/EventHandler.h"
#include <iostream>
#include <mutex>
#include <sys/eventfd.h>
#include <thread>

namespace CppServer {
namespace core {
class EventLoop {
public:
  using Func = std::function<void()>;

private:
  std::thread looperThread_;
  base::BlockingQueue<Func> taskQueue_;
  std::mutex taskQueueMutex_;
  Epoller poller_;

  int eventFd_;
  EventHandler wakeUpHandler;

  void mainLoop() {
    while (true) {
      std::vector<EventHandler *> handlers;
      poller_.pollEvents(handlers);
      for (auto &handler : handlers) {
        handler->handlePendingEvents();
      }
      handlePendingTasks();
    }
  }

  void wake() {
    uint64_t value = 1;
    write(eventFd_, &value, sizeof(value));
  }

  void handlePendingTasks() {
    base::BlockingQueue<Func> currentTasks;
    {
      std::unique_lock<std::mutex> lk = acquireTaskQueueLock();
      currentTasks.swap(taskQueue_);
    }

    std::vector<Func> tasks = currentTasks.popAll();
    for (auto &task : tasks) {
      task();
    }
  }

  std::unique_lock<std::mutex> acquireTaskQueueLock() {
    std::unique_lock<std::mutex> lk(taskQueueMutex_);
    return lk;
  }

public:
  EventLoop() : eventFd_(eventfd(0, 0)), wakeUpHandler(eventFd_) {
    poller_.registerEvent(&wakeUpHandler, EPOLLIN | EPOLLET);
  }

  ~EventLoop() { close(eventFd_); }

  Epoller &getPoller() { return poller_; }

  void loop() { looperThread_ = std::thread(&EventLoop::mainLoop, this); }

  void join() { looperThread_.join(); }

  void post(Func func) {
    std::unique_lock<std::mutex> lk = acquireTaskQueueLock();
    taskQueue_.push(func);
    wake();
  }
};
} // namespace core
} // namespace CppServer

#endif // __PROJECTS_CPP_SERVER_V2_SRC_CORE_EVENTLOOP_H_