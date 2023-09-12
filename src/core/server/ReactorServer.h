#ifndef __PROJECTS_CPP_SERVER_V2_SRC_CORE_SERVER_REACTORSERVER_H_
#define __PROJECTS_CPP_SERVER_V2_SRC_CORE_SERVER_REACTORSERVER_H_

#include "core/Acceptor.h"
#include "core/Epoller.h"
#include "core/EventHandler.h"
#include "core/EventLoop.h"
#include "core/EventLoopPool.h"
#include "core/InetAddress.h"
#include "core/Socket.h"
#include "core/TcpConnection.h"
#include <iostream>
#include <memory>
#include <thread>

namespace CppServer {
namespace core {
class ReactorServer {
private:
  InetAddress *inetAddress_ = nullptr;
  Acceptor acceptor_;
  EventLoop acceptorLoop_;

  EventLoopPool<100> peerEventLoops;

  std::unordered_map<int, std::unique_ptr<TcpConnection<Conn::NonBlocking>>>
      connections;

  std::string handleMessage(const std::string &) {
    return "HTTP/1.1 200 OK\r\n"
           "Content-Length: 0\r\n"
           "\r\n";
  }

public:
  ReactorServer(InetAddress &inetAddress)
      : inetAddress_(&inetAddress), acceptor_(acceptorLoop_, *inetAddress_) {
    acceptor_.onNewConnection([this](int peerFd) {
      // std::cout << "1\n";
      EventLoop &looper = peerEventLoops.getNextLoop();

      looper.post([this, peerFd, &looper]() {
        // std::cout << "2\n";
        auto connection =
            std::make_unique<TcpConnection<Conn::NonBlocking>>(peerFd, &looper);
        connection->onMessage(
            [this](const std::string &req) { return handleMessage(req); });
        connections[peerFd] = std::move(connection);
        // std::cout << "3\n";
      });
    });
  }

  void start() {
    // std::cout << std::this_thread::get_id() << " main-thread\n";
    acceptor_.listen();
  }
};
} // namespace core
} // namespace CppServer

#endif // __PROJECTS_CPP_SERVER_V2_SRC_CORE_SERVER_REACTORSERVER_H_