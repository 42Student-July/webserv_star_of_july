#ifndef CGI_HPP
#define CGI_HPP

#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <sstream>

#include "ConfigDTO.hpp"
#include "HttpRequestDTO.hpp"

const int READ = 0;
const int WRITE = 1;
const size_t BUF_SIZE = 512;

class CGI {
 public:
  CGI(HttpRequestDTO &req, ConfigDTO &conf);
  virtual ~CGI();
  void run();
  void createEnvs();
  void createPipe();
  void spawnChild();
  void dupIO();
  void dupFd(int oldfd, int newfd);
  void createArgs();
  void readCGI();

  void makeCGIResponseLines(std::string cgi_body_);
  std::string getResponseFromCGI() const;

  CGI(CGI const &other);
  CGI &operator=(CGI const &other);

 private:
  //どちらかに統一する
  std::string cgi_body_;
  std::vector<std::string> cgi_response_lines;

  HttpRequestDTO req_;
  ConfigDTO conf_;
  std::string method_;
  std::string exec_path_;
  std::string ip_;
  char **exec_args_;
  char **cgi_envs_;
  int pipe_c2p_[2];
  int pipe_p2c_[2];
  // pid_t child_pid_;
};

#endif
