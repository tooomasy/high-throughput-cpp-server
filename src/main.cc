#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "base/concurrency/SPSCQueue.h"
#include "core/InetAddress.h"
#include "core/server/BasicServer.h"
#include "core/server/BasicServerV2.h"
#include "core/server/EpollServer.h"
#include "core/server/ReactorServer.h"

int main() {
  CppServer::core::InetAddress address("127.0.0.1", 8080);

  // CppServer::core::BasicServer server(address);
  // CppServer::core::BasicServerV2 server(address);
  // CppServer::core::EpollServer server(address);
  CppServer::core::ReactorServer server(address);
  server.start();

  return 0;
}