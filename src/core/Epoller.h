#ifndef __PROJECTS_CPP_SERVER_V2_SRC_CORE_EPOLLER_H_
#define __PROJECTS_CPP_SERVER_V2_SRC_CORE_EPOLLER_H_

#include "core/EventHandler.h"
#include "core/Events.h"
#include <sys/epoll.h>
#include <vector>

namespace CppServer {
namespace core {
class Epoller {
private:
  static inline constexpr int MAX_EVENTS = 1000;

private:
  int fd_;
  std::vector<epoll_event> events;

  void update(int operation, EventHandler *handler, unsigned int targetEvent) {
    epoll_event event;
    event.data.ptr = handler;
    event.events = targetEvent;
    epoll_ctl(fd_, operation, handler->fd(), &event);
  }

public:
  explicit Epoller() : fd_(epoll_create1(0)), events(MAX_EVENTS) {}

  void registerEvent(EventHandler *handler, unsigned int targetEvent) {
    update(EPOLL_CTL_ADD, handler, targetEvent);
  }

  void updateEvent(EventHandler *handler, unsigned int targetEvent) {
    update(EPOLL_CTL_MOD, handler, targetEvent);
  }

  void removeEvent(EventHandler *handler) { update(EPOLL_CTL_DEL, handler, 0); }

  void pollEvents(std::vector<EventHandler *> &handlers) {
    int n = epoll_wait(fd_, events.data(), events.size(), -1);
    for (int i = 0; i < n; ++i) {
      EventHandler *handler = static_cast<EventHandler *>(events[i].data.ptr);
      handler->setPendingEvents(events[i].events);
      handlers.push_back(handler);
    }
  }
};
} // namespace core
} // namespace CppServer

#endif // __PROJECTS_CPP_SERVER_V2_SRC_CORE_EPOLLER_H_