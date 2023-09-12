#ifndef __PROJECTS_CPP_SERVER_V2_SRC_CORE_SERVER_BASICSERVERV2_H_
#define __PROJECTS_CPP_SERVER_V2_SRC_CORE_SERVER_BASICSERVERV2_H_

#include "base/concurrency/BlockingQueue.h"
#include "base/concurrency/ThreadPool.h"
#include "core/InetAddress.h"
#include "core/Socket.h"
#include "core/TcpConnection.h"
namespace CppServer {
namespace core {
class BasicServerV2 {
private:
  InetAddress *inetAddress_ = nullptr;
  Socket serverSocket;

  base::ThreadPool<base::BlockingQueue, 1000> pool;

  std::string handleMessage(const std::string &) {
    return "HTTP/1.1 200 OK\r\n"
           "Content-Length: 0\r\n"
           "\r\n";
  }

  void establishConnection(int peerFd) {
    TcpConnection<Conn::Blocking> connection(peerFd);
    connection.onMessage(
        [this](const std::string &msg) { return handleMessage(msg); });
    connection.process();
  }

public:
  BasicServerV2(InetAddress &inetAddress)
      : inetAddress_(&inetAddress),
        serverSocket(Socket::create(*inetAddress_)) {}

  void start() {
    // serverSocket.setReuseAddress(true);
    // serverSocket.setReusePort(true);
    serverSocket.listen();
    while (true) {
      InetAddress peerAddress;
      int peerFd = serverSocket.accept(&peerAddress);
      pool.addTask(&BasicServerV2::establishConnection, this, peerFd);
    }
    close(serverSocket.fd());
  }
};
} // namespace core
} // namespace CppServer

#endif // __PROJECTS_CPP_SERVER_V2_SRC_CORE_SERVER_BASICSERVERV2_H_