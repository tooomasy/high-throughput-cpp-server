#ifndef __PROJECTS_CPP_SERVER_V2_SRC_CORE_TCPCONNECTION_H_
#define __PROJECTS_CPP_SERVER_V2_SRC_CORE_TCPCONNECTION_H_

#include "base/Buffer.h"
#include "core/Epoller.h"
#include "core/EventHandler.h"
#include "core/EventLoop.h"
#include <iostream>
#include <string>
#include <unistd.h>

namespace CppServer {
namespace core {
enum class Conn { Blocking, NonBlocking };

template <Conn conn> class TcpConnection {};

template <> class TcpConnection<Conn::Blocking> {
public:
  using MessageCallback = std::function<std::string(const std::string &)>;

private:
  int fd_;
  EventHandler eventHandler;

  MessageCallback messageCb_;

  bool isClosed = false;

  std::string inputBuffer_;
  int inBufIdx_ = 0;
  std::string outputBuffer_;
  int outBufIdx_ = 0;

  char tmpBuffer[1024];

  bool handleRead() {
    int n = ::read(fd_, tmpBuffer, sizeof(tmpBuffer));
    if (n == 0) {
      eventHandler.dispatchEvent(Event::Closeable);
      return false;
    }

    if (n > 0) {
      inputBuffer_ += std::string(tmpBuffer, n);
      inBufIdx_ += n;
      return true;
    }
    return false;
  }

  bool handleWrite() {
    int n = ::write(fd_, outputBuffer_.data() + outBufIdx_,
                    outputBuffer_.size() - outBufIdx_);
    if (n > 0) {
      outBufIdx_ += n;
      return true;
    }
    return false;
  }

  void handleClose() { isClosed = true; }

public:
  TcpConnection(int fd) : fd_(fd), eventHandler(fd_) {
    eventHandler.onReadable([this]() { handleRead(); });
    eventHandler.onWriteable([this]() { handleWrite(); });
    eventHandler.onCloseable([this]() { handleClose(); });
  }

  ~TcpConnection() { close(fd_); }

  void onMessage(MessageCallback cb) { messageCb_ = std::move(cb); }

  void process() {
    std::vector<std::function<void()>> tasks{
        [this]() { eventHandler.dispatchEvent(Event::Readable); },
        [this]() { outputBuffer_ = messageCb_(inputBuffer_); },
        [this]() { eventHandler.dispatchEvent(Event::Writeable); },
        [this]() {
          inputBuffer_ = "";
          outputBuffer_ = "";
          inBufIdx_ = 0;
          outBufIdx_ = 0;
        }};
    int taskIdx = 0;
    while (!isClosed) {
      tasks[taskIdx]();
      taskIdx = (taskIdx + 1) % tasks.size();
    }
  }
};

template <> class TcpConnection<Conn::NonBlocking> {
public:
  using MessageCallback = std::function<std::string(const std::string &)>;

private:
  int fd_;
  EventHandler eventHandler;
  EventLoop *loop_ = nullptr;
  Epoller *poller_ = nullptr;

  MessageCallback messageCb_;

  bool isClosed = false;

  base::Buffer inputBuf_;
  base::Buffer outputBuf_;

  // FIXME: assume one call to retrieve all
  bool handleRead() {
    int n = inputBuf_.read(fd_);
    if (n <= 0) {
      eventHandler.dispatchEvent(Event::Closeable);
      return false;
    }
    return true;
  }

  // FIXME: assume one call to retrieve all
  bool handleWrite() {
    int n = outputBuf_.write(fd_);
    if (n >= 0) {
      outputBuf_.flush();
      return true;
    }
    return false;
  }

  void handleClose() { isClosed = true; }

public:
  TcpConnection(int fd, Epoller &epoller)
      : fd_(fd), eventHandler(fd_), loop_(nullptr), poller_(&epoller) {
    init();
  }

  TcpConnection(int fd, EventLoop *loop)
      : fd_(fd), eventHandler(fd_), loop_(loop),
        poller_(loop_ ? &loop_->getPoller() : nullptr) {
    init();
  }

  ~TcpConnection() { close(fd_); }

  void init() {
    eventHandler.onReadable([this]() {
      if (handleRead()) {
        outputBuf_.put(messageCb_(inputBuf_.retrieveAll()));
        poller_->updateEvent(&eventHandler, EPOLLOUT | EPOLLET);
      }
    });
    eventHandler.onWriteable([this]() {
      if (handleWrite()) {
        poller_->updateEvent(&eventHandler, EPOLLIN | EPOLLET);
      }
    });
    eventHandler.onCloseable([this]() {
      handleClose();
      poller_->removeEvent(&eventHandler);
    });

    poller_->registerEvent(&eventHandler, EPOLLIN | EPOLLET);
  }

  void onMessage(MessageCallback cb) { messageCb_ = std::move(cb); }
};
} // namespace core
} // namespace CppServer

#endif // __PROJECTS_CPP_SERVER_V2_SRC_CORE_TCPCONNECTION_H_