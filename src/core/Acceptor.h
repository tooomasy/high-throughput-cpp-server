#ifndef __PROJECTS_CPP_SERVER_V2_SRC_CORE_ACCEPTOR_H_
#define __PROJECTS_CPP_SERVER_V2_SRC_CORE_ACCEPTOR_H_

#include "core/Epoller.h"
#include "core/EventHandler.h"
#include "core/EventLoop.h"
#include "core/InetAddress.h"
#include "core/Socket.h"
#include <iostream>

namespace CppServer {
namespace core {
class Acceptor {
public:
  using NewConnectionCallback = std::function<void(int)>;

private:
  EventLoop *looper_ = nullptr;
  InetAddress *inetAddress_ = nullptr;

  Socket serverSocket_;
  EventHandler serverHandler_;

  NewConnectionCallback newConnectionCb;

public:
  explicit Acceptor(EventLoop &looper, InetAddress &inetAddr)
      : looper_(&looper), inetAddress_(&inetAddr),
        serverSocket_(Socket::create(*inetAddress_)),
        serverHandler_(serverSocket_.fd()) {
    serverHandler_.onReadable([this]() {
      looper_->post([this]() {
        InetAddress peerAddress;
        int peerFd = serverSocket_.accept(&peerAddress);
        newConnectionCb(peerFd);
      });
    });
  }

  void listen() {
    serverSocket_.listen();
    looper_->getPoller().registerEvent(&serverHandler_, EPOLLIN);
    looper_->loop();
    looper_->join();
  }

  void onNewConnection(NewConnectionCallback cb) {
    newConnectionCb = std::move(cb);
  }
};
} // namespace core
} // namespace CppServer

#endif // __PROJECTS_CPP_SERVER_V2_SRC_CORE_ACCEPTOR_H_