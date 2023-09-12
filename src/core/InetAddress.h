#ifndef __PROJECTS_CPP_SERVER_V2_SRC_CORE_INETADDRESS_H_
#define __PROJECTS_CPP_SERVER_V2_SRC_CORE_INETADDRESS_H_

#include <iostream>
#include <netinet/in.h>
#include <string>

namespace CppServer {
namespace core {
class InetAddress {
private:
  std::string ipAddress_;
  int port_;

public:
  explicit InetAddress(const std::string &ipAddress = "", const int port = 0)
      : ipAddress_(ipAddress), port_(port) {}

  const std::string &ip() const { return ipAddress_; }
  const int &port() const { return port_; }

  void setIp(const std::string &ip) { ipAddress_ = ip; }
  void setPort(const int port) { port_ = port; }

  friend std::ostream &operator<<(std::ostream &os, const InetAddress &addr) {
    // clang-format off
    os << "InetAddress[\n"
          "\tipAddress_=" << addr.ip() << "\n"
          "\tport_=" << addr.port() << "\n"
          "]";
    // clang-format on
    return os;
  }
};
} // namespace core
} // namespace CppServer

#endif // __PROJECTS_CPP_SERVER_V2_SRC_CORE_INETADDRESS_H_