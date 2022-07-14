#ifndef SRC_CONNECTIONSOCKET_HPP_
#define SRC_CONNECTIONSOCKET_HPP_

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <string>

#include "ASocket.hpp"
#include "HttpRequestDTO.hpp"
#include "HttpRequestParser.hpp"
#include "HttpResponse.hpp"

class ConnectionSocket : public ASocket {
 public:
  enum State {
    READ,
    WRITE,
    CLOSE,
  };

  ConnectionSocket(int accepted_fd, const ServerConfig &serverconfig);
  ~ConnectionSocket();

  void handleCommunication();
  State getState() const;

 private:
  static const int kRecvBufferSize = (1 << 16);
  // 65536, httpServerだとリクエストの文字数の上限かな

  ConnectionSocket();
  ConnectionSocket(const ConnectionSocket &other);
  ConnectionSocket &operator=(const ConnectionSocket &other);

  void handleReadEvent();
  void handleWriteEvent();
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

#endif  // SRC_CONNECTIONSOCKET_HPP_
