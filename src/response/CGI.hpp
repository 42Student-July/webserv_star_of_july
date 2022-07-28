#ifndef CGI_HPP
#define CGI_HPP

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>

#include "ConfigDTO.hpp"
#include "HttpRequestDTO.hpp"
#include "Path.hpp"
#include "../utility/utility.hpp"

class CGI {
 public:
  CGI();
  virtual ~CGI();
  void run(HttpRequestDTO &req, ConfigDTO &conf, Path &path);
  char **createEnvs(Path &path);
  char **createArgs(Path &path);
  void createPipe();
  std::string buildCGIResponse(const char *exec_command_path, char *const *exec_args, char *const *exec_envs);
  void dupIO();
  void dupFd(int oldfd, int newfd);
  void readCGI();
  char *allocStr(const std::string &str);
  void throwclose(int fd);

  void makeCGIResponseLines(std::string cgi_body_);
  std::string getResponseFromCGI() const;

 private:
  static const int BUF_SIZE = 8192;
  static const std::map<std::string, std::string> EXEC_COMMANDS;
  static const std::map<std::string, std::string> EXEC_PATHS;
  static std::map<std::string, std::string> setExecutePaths();
  static std::map<std::string, std::string> setExecuteCommands();

  std::string cgi_response_;

  HttpRequestDTO req_;
  ConfigDTO conf_;
  std::string method_;
  std::string exec_path_;
  std::string ip_;
  //char **exec_args_;
  //char **cgi_envs_;
  int pipe_c2p_[2];
  int pipe_p2c_[2];
  // pid_t child_pid_;

  //使わない関数
  CGI(CGI const &other);
  CGI &operator=(CGI const &other);
};

#endif
