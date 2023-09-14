#ifndef __PROJECTS_CPP_SERVER_V2_SRC_CORE_SOCKET_H_
#define __PROJECTS_CPP_SERVER_V2_SRC_CORE_SOCKET_H_

#include "core/InetAddress.h"

#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

namespace CppServer {
namespace core {
class Socket {
private:
  int fd_;

  explicit Socket(int fd) : fd_(fd) {}

  void setOption(int option, int val) {
    fcntl(fd_, SOL_SOCKET, option, &val, sizeof(val));
  }

public:
  static Socket create(const InetAddress &address) {
    int socketFd = socket(PF_INET, SOCK_STREAM, 0);
    Socket sock(socketFd);
    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(address.ip().c_str());
    addr.sin_port = htons(address.port());
    sock.setReuseAddress(true);
    sock.setReusePort(true);
    ::bind(socketFd, (sockaddr *)(&addr), sizeof(addr));
    return sock;
  }

  ~Socket() { close(fd_); }

  int fd() const { return fd_; }

  void listen() { ::listen(fd_, 10000); }

  int accept(InetAddress *peerInetAddress) {
    sockaddr_in addr = {};
    socklen_t addrSize = 0;
    int fd = ::accept(fd_, (sockaddr *)&addr, &addrSize);
    peerInetAddress->setIp(inet_ntoa(addr.sin_addr));
    peerInetAddress->setPort(ntohs(addr.sin_port));
    return fd;
  }

  void setReuseAddress(bool enable) {
    setOption(SO_REUSEADDR, static_cast<int>(enable));
  }

  void setReusePort(bool enable) {
    setOption(SO_REUSEPORT, static_cast<int>(enable));
  }
};
} // namespace core
} // namespace CppServer

#endif // __PROJECTS_CPP_SERVER_V2_SRC_CORE_SOCKET_H_