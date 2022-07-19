#ifndef CGI_HPP
#define CGI_HPP

#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sstream>

#include "ConfigDTO.hpp"
#include "HttpRequestDTO.hpp"
#include "Path.hpp"

class CGI {
 public:
  CGI();
  virtual ~CGI();
  void run(HttpRequestDTO &req, ConfigDTO &conf, Path &path);
  void createEnvs(Path &path);
  void createArgs(Path &path);
  void createPipe();
  void spawnChild();
  void dupIO();
  void dupFd(int oldfd, int newfd);
  void readCGI();
  char *allocString(const std::string &str);

  void makeCGIResponseLines(std::string cgi_body_);
  std::string getResponseFromCGI() const;

  CGI(CGI const &other);
  CGI &operator=(CGI const &other);

 private:
  std::string cgi_body_;

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
