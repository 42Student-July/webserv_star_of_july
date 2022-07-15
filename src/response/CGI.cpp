#include "CGI.hpp"
#include <unistd.h>

CGI::CGI(HttpRequestDTO &req, ConfigDTO &conf)
	:req_(req), conf_(conf)
{
}

CGI::~CGI()
{
}

void CGI::run()
{
	createEnvs();
	//createArgs();
	//createArgsにする
	std::string command = "/usr/bin/python3";
	std::string command2 = "cgi-bin/test.py";
	exec_args_ = new char *[3];
	//const外しはしないようにする
	exec_args_[0] = const_cast< char * >(command.c_str());
	exec_args_[1] = const_cast< char * >(command2.c_str());
	exec_args_[2] = NULL;
	createPipe();
	spawnChild();
	
}

std::string CGI::getResponseFromCGI() const
{
	return cgi_body_;
}

void CGI::readCGI()
{
	size_t size = 0;
	char buf[BUF_SIZE];
	memset(buf, 0, sizeof(buf));
	size = read(pipe_c2p_[READ], buf, BUF_SIZE - 1);

}

void print_char(char **c)
{
	for (int i = 0; c[i]; i++)
		printf("%s\n",c[i]);
}

char ** map2Array(std::map<std::string, std::string> map_env, std::string connector)
{
	char ** array_env = NULL;
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

void CGI::createArgs()
{
	std::string command = "/usr/bin/python3";
	std::string command2 = "cgi-bin/test.py";
	exec_args_ = new char *[3];
	//const外しはしないようにする
	exec_args_[0] = const_cast< char * >(command.c_str());
	exec_args_[1] = const_cast< char * >(command2.c_str());
	exec_args_[2] = NULL;
}

void CGI::createEnvs()
{
	std::map< std::string, std::string > map_env;
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
	map_env["QUERY_STRING"] = "";
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
	//print_char(cgi_envs_);
}

void CGI::createPipe()
{
	if (req_.method == "POST") {
		if (pipe(pipe_p2c_) < 0) {
			throw -1;
		}
	}
	if (pipe(pipe_c2p_) < 0) {
		throw -1;
	}
}

void CGI::spawnChild()
{
	pid_t pid = fork();
	if (pid < 0) {
		throw (-1);
	}
	if (pid == 0) {
		//child process
		dupIO();
		int a = execve(exec_args_[0], exec_args_, cgi_envs_);
		if (a < 0) {
			std::cerr << "error: a = " << a << std::endl; 
		}
		exit(1);
	} else {
		//parent_process
		if (req_.method == "POST") {
			close(pipe_p2c_[READ]);
		}
		close(pipe_c2p_[WRITE]);

		write(pipe_p2c_[WRITE], req_.body.c_str(), req_.body.length());

		char buf[BUF_SIZE];
		memset(buf, 0, sizeof(buf));
		ssize_t read_size = 0;
		read_size = read(pipe_c2p_[READ], buf, BUF_SIZE);
		buf[read_size] = '\0';
		cgi_body_ = buf;
	}
}

void CGI::dupIO()
{
	if (req_.method == "POST")
	{
		dupFd(pipe_p2c_[READ], STDIN_FILENO);
		close(pipe_p2c_[WRITE]);
		//ここ要検討
		//close(STDIN_FILENO);
	}
	dupFd(pipe_c2p_[WRITE], STDOUT_FILENO);
	close(pipe_c2p_[READ]);
	//ここ要検討
	//close(STDOUT_FILENO);
}

void CGI::dupFd(int oldfd, int newfd)
{
	if (dup2(oldfd, newfd) < 0) {
		throw -1;
	}
}

CGI::CGI(CGI const &other)
{
    *this = other;
}

CGI &CGI::operator=(CGI const &other)
{
    if (this != &other)
    {
    }
    return *this;
}
