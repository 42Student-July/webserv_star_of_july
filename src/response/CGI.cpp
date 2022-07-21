#include "CGI.hpp"

#include <unistd.h>

#include <iostream>

CGI::CGI() {}
CGI::~CGI() {}

//libに入れる
void freeArrays(char **arrays)
{
	size_t	idx = 0;

	if (!arrays)
		return ;
	while (arrays[idx])
	{
		free(arrays[idx]);
		idx += 1;
	}
	free(arrays);
}

//buildresponse
void CGI::run(HttpRequestDTO &req, ConfigDTO &conf, Path &path) {
  //init
  req_ = req;
  conf_ = conf;

  //setup
  char **exec_args = createArgs(path);
  char **exec_envs = createEnvs(path);

  // execve
  cgi_response_ = buildCGIResponse(exec_args, exec_envs);

  // free
  freeArrays(exec_args);
  freeArrays(exec_envs);
}

std::string CGI::getResponseFromCGI() const { return cgi_response_; }

void CGI::readCGI() {
  size_t size = 0;
  char buf[BUF_SIZE];
  memset(buf, 0, sizeof(buf));
  size = read(pipe_c2p_[0], buf, BUF_SIZE - 1);
  (void)size;
  // unused parameterでコンパイルできなかったので
  // error処理した方がよさそうです
}

char *CGI::allocStr(const std::string &str) {
  char *ret = strdup(str.c_str());
  if (ret == NULL) {
    throw -1;
  }
  return ret;
}

void print_char(char **c) {
  for (int i = 0; c[i]; i++) printf("%s\n", c[i]);
}

char **map2Array(std::map<std::string, std::string> map_env,
                 std::string connector) {
  char **array_env = NULL;
  array_env = new char *[map_env.size() + 1];
  size_t i = 0;
  std::map<std::string, std::string>::const_iterator it = map_env.begin();
  for (; it != map_env.end(); it++, i++) {
    std::string env_str = it->first + connector + it->second;
    array_env[i] = strdup(env_str.c_str());
  }
  array_env[i] = NULL;
  return array_env;
}

char ** CGI::createArgs(Path &path) {
  //もっと良いやり方に変える
  std::string command;
  if (path.getExtension() == ".py") {
    command = "/usr/bin/python3";
  } else if (path.getExtension() == ".pl") {
    command = "/usr/bin/perl";
  } else {
    throw -1;
  }

  // ToDo: ここをgetRawPathではなくlocal_path的なものに変える
  std::string exec_path = path.getRawPath().substr(1);
  std::vector<std::string> path_args = path.getArgs();
  int args_size = path_args.size() + 3;

  char ** exec_args = new char *[args_size];
  int i = 0;
  exec_args[i++] = allocStr(command);
  exec_args[i++] = allocStr(exec_path);

  std::vector<std::string>::iterator it_arg = path_args.begin();
  for (; it_arg != path_args.end(); ++it_arg) {
    exec_args[i++] = allocStr(*it_arg);
  }
  exec_args[i] = NULL;
  return exec_args;
}

char ** CGI::createEnvs(Path &path) {
  std::map<std::string, std::string> map_env;
  map_env["AUTH_TYPE"] = req_.authorization;
  map_env["CONTENT_LENGTH"] = req_.content_length;
  map_env["CONTENT_TYPE"] = req_.content_type;
  map_env["GATEWAY_INTERFACE"] = "CGI/1.1";
  map_env["HTTP_ACCEPT"] = req_.accept;
  map_env["HTTP_FORWARDED"] = req_.forwarded;
  map_env["HTTP_REFERER"] = req_.referer;
  map_env["HTTP_USER_AGENT"] = req_.user_agent;
  map_env["HTTP_X_FORWARDED_FOR"] = req_.x_forwarded_for;
  map_env["PATH_INFO"] = req_.path;
  //ここわからないので後ほど調べる
  map_env["PATH_TRANSLATED"] = "";
  //一旦空白
  map_env["QUERY_STRING"] = path.getQuery();
  map_env["REMOTE_ADDR"] = "127.0.0.1";
  //要修正
  map_env["REMOTE_HOST"] = "webserv.com";
  //一旦空白
  map_env["REMOTE_IDENT"] = "";
  map_env["REMOTE_USER"] = "";

  map_env["REQUEST_METHOD"] = req_.method;
  //要パース
  map_env["SCRIPT_NAME"] = "";
  map_env["SERVER_NAME"] = conf_.server;
  std::stringstream ss;
  ss << conf_.port;
  map_env["SERVER_PORT"] = ss.str();
  map_env["SERVER_PROTOCOL"] = "HTTP/1.1";
  map_env["SERVER_SOFTWARE"] = "webserv/1.0";

  char ** exec_envs = map2Array(map_env, "=");
  return exec_envs;
}

void CGI::createPipe() {
  if (pipe(pipe_p2c_) < 0) {
    throw -1;
  }
  if (pipe(pipe_c2p_) < 0) {
    throw -1;
  }
}

void throwclose(int fd) {
  if (close(fd) == -1) {
    throw -1;
  }
}

std::string CGI::buildCGIResponse(char **exec_args, char **exec_envs) {
  char buf[BUF_SIZE];
  memset(buf, 0, sizeof(buf));
  // リファクターする
  createPipe();
  pid_t pid = fork();
  if (pid < 0) {
    std::cerr << "fork" << std::endl;
    throw(-1);
  }
  if (pid == 0) {
    // child process
    dupFd(pipe_p2c_[0], STDIN_FILENO);
    throwclose(pipe_p2c_[0]);
    throwclose(pipe_p2c_[1]);

    dupFd(pipe_c2p_[1], STDOUT_FILENO);
    throwclose(pipe_c2p_[1]);
    throwclose(pipe_c2p_[0]);

    int a = execve(exec_args[0], exec_args, exec_envs);
    if (a < 0) {
		throw -1;
    }
  } else {
    // parent_process
    throwclose(pipe_p2c_[0]);
    throwclose(pipe_c2p_[1]);

    write(pipe_p2c_[1], req_.body.c_str(), req_.body.length());
    throwclose(pipe_p2c_[1]);

    ssize_t read_size = 0;
    read_size = read(pipe_c2p_[0], buf, BUF_SIZE);
    throwclose(pipe_c2p_[0]);
    buf[read_size] = '\0';
  }
  return buf;
}

/* void CGI::dupIO() { */

/*   if (req_.method == "POST") { */
/*     dupFd(pipe_p2c_[0], STDIN_FILENO); */
/* 	close(pipe_p2c_[0]); */
/*     close(pipe_p2c_[1]); */
/*     //ここ要検討 */
/*     // close(STDIN_FILENO); */
/*   } */
/*   dupFd(pipe_c2p_[1], STDOUT_FILENO); */
/*   close(pipe_c2p_[1]); */
/*   close(pipe_c2p_[0]); */
/*   //ここ要検討 */
/*   // close(STDOUT_FILENO); */
/* } */

void CGI::dupFd(int oldfd, int newfd) {
  if (dup2(oldfd, newfd) < 0) {
    throw -1;
  }
}

CGI::CGI(CGI const &other) { *this = other; }

CGI &CGI::operator=(CGI const &other) {
  if (this != &other) {
  }
  return *this;
}
