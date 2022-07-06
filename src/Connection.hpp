#ifndef HTTPSERVER_SRC_CONNECTION_HPP_
#define HTTPSERVER_SRC_CONNECTION_HPP_

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <string>

#include "ASocket.hpp"
#include "HttpRequest.hpp"
#include "HttpRequestParser.hpp"
#include "HttpResponse.hpp"
#include "SocketObserver.hpp"

class Connection : public ASocket {
public:
  explicit Connection(int accepted_fd);
  ~Connection();

  void communicateWithClient(SocketObserver *observer);

private:
  enum State {
    READ,
    WRITE,
    CLOSE,
  };
  static const int kRecvBufferSize = (1 << 16);
  // 65536, httpServerだとリクエストの文字数の上限かな

  Connection();
  Connection(const Connection &other);
  Connection &operator=(const Connection &other);

  void handleReadEvent(SocketObserver *observer);
  void handleWriteEvent(SocketObserver *observer);
  ssize_t recvFromClient();
  void generateRequest(ssize_t recv_size);
  void generateResponse(ssize_t recv_size);
  void sendResponse() const;

  // int socket_fd_;
  State state_;
  char recv_buffer_[kRecvBufferSize + 1];
  std::string response_;
  // この辺の変数のスコープと型（ポインタにするかしないかは後で考える)
  HttpRequestParser request_parser_;
  HttpRequest *current_request_;
  HttpResponse *current_response_;
};

#endif // HTTPSERVER_SRC_CONNECTION_HPP_
