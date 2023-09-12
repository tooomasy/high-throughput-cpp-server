#ifndef __PROJECTS_CPP_SERVER_V2_SRC_CORE_SERVER_SERVER_H_
#define __PROJECTS_CPP_SERVER_V2_SRC_CORE_SERVER_SERVER_H_

#include <functional>
namespace CppServer {
namespace core {
class Server {
protected:
  using NewConnectionCallback = std::function<void()>;
  using ConnectionReadableCallback = std::function<void()>;
  using ConnectionWriteableCallback = std::function<void()>;

  NewConnectionCallback newConnectionCb = []() {};
  ConnectionReadableCallback connectionReadableCb = []() {};
  ConnectionWriteableCallback connectionWriteableCb = []() {};
};
} // namespace core
} // namespace CppServer

#endif // __PROJECTS_CPP_SERVER_V2_SRC_CORE_SERVER_SERVER_H_