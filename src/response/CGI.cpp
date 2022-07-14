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

std::string CGI::getBodyFromCGI() const
{
	return cgi_body_;
}

void CGI::readCGI()
{
	size_t size = 0;
	char buf[BUF_SIZE];
	memset(buf, 0, sizeof(buf));
	size = read(pipe_child2parent_[READ], buf, BUF_SIZE - 1);

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
	map_env["AUTH_TYPE"] = "";
	map_env["CONTENT_LENGTH"] = "";
	map_env["CONTENT_TYPE"] = "application/x-www-form-urlencoded";
	map_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	map_env["HTTP_ACCEPT"] = "";
	map_env["HTTP_FORWARDED"] = "";
	map_env["HTTP_REFERER"] = "";
	map_env["HTTP_USER_AGENT"] = "";
	map_env["HTTP_X_FORWARDED_FOR"] = "";
	map_env["PATH_INFO"] = "";
	map_env["PATH_TRANSLATED"] = "";
	map_env["QUERY_STRING"] = "";
	map_env["REMOTE_ADDR"] = "127.0.0.1";
	map_env["REMOTE_HOST"] = "abc.com";
	map_env["REMOTE_IDENT"] = "";
	map_env["REMOTE_USER"] = "";
	map_env["REQUEST_METHOD"] = "GET";
	map_env["SCRIPT_NAME"] = "";
	map_env["SERVER_NAME"] = "";
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
		if (pipe(pipe_parent2child_) < 0) {
			throw -1;
		}
	}
	if (pipe(pipe_child2parent_) < 0) {
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
		IODup();
		int a = execve(exec_args_[0], exec_args_, cgi_envs_);
		if (a < 0) {
			std::cerr << "error: a = " << a << std::endl; 
		}
		exit(1);
	} else {
		//parent_process
		if (req_.method == "POST") {
			close(pipe_parent2child_[READ]);
		}
		close(pipe_child2parent_[WRITE]);

		write(pipe_parent2child_[WRITE], req_.body.c_str(), req_.body.length());

		char buf[BUF_SIZE];
		memset(buf, 0, sizeof(buf));
		ssize_t read_size = 0;
		read_size = read(pipe_child2parent_[READ], buf, BUF_SIZE);
		buf[read_size] = '\0';
		cgi_body_ = buf;
	}
}

void CGI::IODup()
{
	if (req_.method == "POST")
	{
		dupFd(pipe_parent2child_[READ], STDIN_FILENO);
		close(pipe_parent2child_[WRITE]);
		//ここ要検討
		//close(STDIN_FILENO);
	}
	dupFd(pipe_child2parent_[WRITE], STDOUT_FILENO);
	close(pipe_child2parent_[READ]);
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
