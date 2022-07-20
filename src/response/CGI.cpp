#include "CGI.hpp"

#include <unistd.h>
#include <iostream>

CGI::CGI() {}
CGI::~CGI() {}

void CGI::run(HttpRequestDTO &req, ConfigDTO &conf, Path &path) {
  req_ = req;
  conf_ = conf;
  createEnvs(path);
  createArgs(path);
  createPipe();
  spawnChild();
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

char *CGI::allocStr(const std::string &str)
{
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

void CGI::createArgs(Path &path) {
  //もっと良いやり方に変える
  std::string command;
  if (path.getExtension() == ".py") {
	command = "/usr/bin/python3";
  } else if (path.getExtension() == ".pl") {
    command = "/usr/bin/perl";
  } else {
	  throw -1;
  }

  //ToDo: ここをgetRawPathではなくlocal_path的なものに変える
  std::string exec_path = path.getRawPath().substr(1);
  std::vector<std::string> path_args = path.getArgs();
  int args_size = path_args.size() + 3;

  exec_args_ = new char *[args_size];
  int i = 0;
  exec_args_[i++] = allocStr(command);
  exec_args_[i++] = allocStr(exec_path);

  std::vector<std::string>::iterator it_arg = path_args.begin();
  for (; it_arg != path_args.end(); ++it_arg) {
	  exec_args_[i++] = allocStr(*it_arg);
  }
  exec_args_[i] = NULL;
}

void CGI::createEnvs(Path &path) {
	(void)path;
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

  cgi_envs_ = map2Array(map_env, "=");
  // print_char(cgi_envs_);
}

void CGI::createPipe() {
  if (req_.method == "POST") {
    if (pipe(pipe_p2c_) < 0) {
      throw -1;
    }
  }
  if (pipe(pipe_c2p_) < 0) {
    throw -1;
  }
}

void CGI::spawnChild() {
  //headerで宣言する

  pid_t pid = fork();
  if (pid < 0) {
	std::cerr << "fork" << std::endl;
    throw(-1);
  }
  if (pid == 0) {
    // child process
    //dupIO();
	if (req_.method == "POST") {
	
		dup2(pipe_p2c_[0], STDIN_FILENO);
		close(pipe_p2c_[0]);
		close(pipe_p2c_[1]);

	}
	dupFd(pipe_c2p_[1], STDOUT_FILENO);
	close(pipe_c2p_[1]);
	close(pipe_c2p_[0]);

	/* char tmp[BUF_SIZE]; */ 
	/* read(STDIN_FILENO, tmp, BUF_SIZE); */
	/* std::cerr << "tmp: " << tmp << std::endl; */

	int a = execve(exec_args_[0], exec_args_, cgi_envs_);
    if (a < 0) {
      std::cerr << "error: a = " << a << std::endl;
    }
  } else {
	//deleteEnviron
    // parent_process
    close(pipe_p2c_[0]);
    close(pipe_c2p_[1]);

	//int wstatus = 0;
	/* std::cout << "req: " << req_.body.c_str() << std::endl; */
    write(pipe_p2c_[1], req_.body.c_str(), req_.body.length());
	close(pipe_p2c_[1]);
	//int wait_pid = wait(&wstatus);

    char buf[BUF_SIZE];
    memset(buf, 0, sizeof(buf));
    ssize_t read_size = 0;
    read_size = read(pipe_c2p_[0], buf, BUF_SIZE);
	close(pipe_c2p_[0]);
    buf[read_size] = '\0';
    cgi_response_ = buf;
  }
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
