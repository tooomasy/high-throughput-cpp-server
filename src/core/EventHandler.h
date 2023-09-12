#ifndef __PROJECTS_CPP_SERVER_V2_SRC_CORE_CHANNEL_H_
#define __PROJECTS_CPP_SERVER_V2_SRC_CORE_CHANNEL_H_

#include "core/Events.h"
#include <functional>
#include <sys/epoll.h>
namespace CppServer {
namespace core {
class EventHandler {
public:
  static inline auto DefaultCallback = []() {};
  using ReadableCallback = std::function<void()>;
  using WritableCallback = ReadableCallback;
  using CloseableCallback = ReadableCallback;

private:
  int fd_ = -1;
  unsigned int pendingEvents = 0;

  ReadableCallback readableCb_ = DefaultCallback;
  WritableCallback writeableCb_ = DefaultCallback;
  CloseableCallback closeableCb_ = DefaultCallback;

  static Event eventAdaptor(unsigned int epollEvent) {
    // clang-format off
    if (epollEvent & EPOLLIN) return Event::Readable;
    if (epollEvent & EPOLLOUT) return Event::Writeable;
    return Event::None;
    // clang-format on
  }

public:
  explicit EventHandler(int fd) : fd_(fd) {}

  int fd() const { return fd_; }

  void onReadable(ReadableCallback cb) { readableCb_ = std::move(cb); }
  void onWriteable(WritableCallback cb) { writeableCb_ = std::move(cb); }
  void onCloseable(CloseableCallback cb) { closeableCb_ = std::move(cb); }

  void setPendingEvents(unsigned int events) { pendingEvents = events; }

  void handlePendingEvents() {
    dispatchEvent(EventHandler::eventAdaptor(pendingEvents));
  }

  void dispatchEvent(Event event) {
    switch (event) {
    case Event::Readable:
      readableCb_();
      break;
    case Event::Writeable:
      writeableCb_();
      break;
    case Event::Closeable:
      closeableCb_();
      break;
    case Event::Connected:
      break;
    case Event::None:
      break;
    }
  }
};
} // namespace core
} // namespace CppServer

#endif // __PROJECTS_CPP_SERVER_V2_SRC_CORE_CHANNEL_H_