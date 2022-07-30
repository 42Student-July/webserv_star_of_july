#ifndef SRC_CLIENTSOCKET_HPP_
#define SRC_CLIENTSOCKET_HPP_

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <string>

#include "ASocket.hpp"
#include "ConfigConverter.hpp"
#include "HttpRequestConverter.hpp"
#include "HttpRequestDTO.hpp"
#include "HttpRequestParser.hpp"
#include "MessageBodyParser.hpp"
#include "response.h"

class ClientSocket : public ASocket {
 public:
  enum State { READ, WRITE, CLOSE };

  ClientSocket(int accepted_fd, const ServerConfig &serverconfig);
  ~ClientSocket();

  void handleReadEvent();
  void handleWriteEvent();
  State getState() const;

 private:
  static const int kRecvBufferSize = (1 << 16);
  // 65536, httpServerだとリクエストの文字数の上限かな

  ClientSocket();
  ClientSocket(const ClientSocket &other);
  ClientSocket &operator=(const ClientSocket &other);

  ssize_t recvFromClient();
  void generateRequest(ssize_t recv_size);
  void generateResponse();
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

#endif  // SRC_CLIENTSOCKET_HPP_
