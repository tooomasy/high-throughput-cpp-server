#ifndef __PROJECTS_CPP_SERVER_V2_SRC_CORE_SERVER_EPOLLSERVER_H_
#define __PROJECTS_CPP_SERVER_V2_SRC_CORE_SERVER_EPOLLSERVER_H_

#include "core/Epoller.h"
#include "core/EventHandler.h"
#include "core/InetAddress.h"
#include "core/Socket.h"
#include "core/TcpConnection.h"
#include <iostream>
#include <memory>
#include <sys/epoll.h>
#include <unordered_map>

namespace CppServer {
namespace core {
class EpollServer {
private:
  InetAddress *inetAddress_ = nullptr;
  Epoller epoller;

  Socket serverSocket;
  EventHandler serverHandler;
  std::unordered_map<int, std::unique_ptr<TcpConnection<Conn::NonBlocking>>>
      connections;

  std::string handleMessage(const std::string &) {
    return "HTTP/1.1 200 OK\r\n"
           "Content-Length: 0\r\n"
           "\r\n";
  }

public:
  EpollServer(InetAddress &inetAddress)
      : inetAddress_(&inetAddress), serverSocket(Socket::create(*inetAddress_)),
        serverHandler(serverSocket.fd()) {
    serverHandler.onReadable([this]() {
      InetAddress peerAddress;
      int peerFd = serverSocket.accept(&peerAddress);
      auto connection =
          std::make_unique<TcpConnection<Conn::NonBlocking>>(peerFd, epoller);
      connection->onMessage(
          [this](const std::string &req) { return handleMessage(req); });
      connections[peerFd] = std::move(connection);
      // // std::cout << peerFd << '\n';
    });
  }

  void start() {
    serverSocket.listen();
    epoller.registerEvent(&serverHandler, EPOLLIN);

    while (true) {
      std::vector<EventHandler *> handlers;
      epoller.pollEvents(handlers);
      for (auto &handler : handlers) {
        handler->handlePendingEvents();
      }
    }
    close(serverSocket.fd());
  }
};
} // namespace core
} // namespace CppServer

#endif // __PROJECTS_CPP_SERVER_V2_SRC_CORE_SERVER_EPOLLSERVER_H_