#ifndef __PROJECTS_CPP_SERVER_V2_SRC_CORE_SERVER_BASICSERVER_H_
#define __PROJECTS_CPP_SERVER_V2_SRC_CORE_SERVER_BASICSERVER_H_

#include "core/EventHandler.h"
#include "core/InetAddress.h"
#include "core/Socket.h"
#include "core/TcpConnection.h"
#include <iostream>

namespace CppServer {
namespace core {
class BasicServer {
private:
  InetAddress *inetAddress_ = nullptr;
  Socket serverSocket;

  std::string handleMessage(const std::string &) {
    return "HTTP/1.1 200 OK\r\n"
           "Content-Length: 0\r\n"
           "\r\n";
  }

public:
  BasicServer(InetAddress &inetAddress)
      : inetAddress_(&inetAddress),
        serverSocket(Socket::create(*inetAddress_)) {}

  void start() {
    // serverSocket.setReuseAddress(true);
    // serverSocket.setReusePort(true);
    serverSocket.listen();
    while (true) {
      InetAddress peerAddress;
      int peerFd = serverSocket.accept(&peerAddress);

      TcpConnection<Conn::Blocking> connection(peerFd);
      connection.onMessage(
          [this](const std::string &msg) { return handleMessage(msg); });
      connection.process();
    }
    close(serverSocket.fd());
  }
};
} // namespace core
} // namespace CppServer

#endif // __PROJECTS_CPP_SERVER_V2_SRC_CORE_SERVER_BASICSERVER_H_