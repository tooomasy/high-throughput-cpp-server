#ifndef __PROJECTS_CPP_SERVER_V2_SRC_BASE_BUFFER_H_
#define __PROJECTS_CPP_SERVER_V2_SRC_BASE_BUFFER_H_

#include <string>
#include <unistd.h>

namespace CppServer {
namespace base {
class Buffer {
private:
  char tmpBuffer_[1024] = {};
  std::string buffer_;
  int idx = 0;

public:
  std::string retrieveAll() {
    std::string res = std::move(buffer_);
    flush();
    return res;
  }

  int read(int fd) {
    int n = ::read(fd, tmpBuffer_, sizeof(tmpBuffer_));
    if (n >= 0) {
      buffer_ += std::string(tmpBuffer_, n);
      idx += n;
      return n;
    }
    return -1;
  }

  int write(int fd) {
    int n = ::write(fd, buffer_.data() + idx, buffer_.size() - idx);
    if (n >= 0) {
      idx += n;
      return n;
    }
    return -1;
  }

  void put(const std::string &value) {
    flush();
    buffer_ = value;
  }

  void flush() { idx = 0; }
};
} // namespace base
} // namespace CppServer

#endif // __PROJECTS_CPP_SERVER_V2_SRC_BASE_BUFFER_H_