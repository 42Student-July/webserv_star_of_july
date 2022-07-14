#ifndef CGI_HPP
# define CGI_HPP

#include "ConfigDTO.hpp"
#include "HttpRequestDTO.hpp"
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <sys/types.h>

const int READ = 0;
const int WRITE = 1;
const size_t BUF_SIZE = 512;

class CGI
{
    public:
        CGI(HttpRequestDTO &req, ConfigDTO &conf);
        virtual ~CGI();
		void run();
		void createEnvs();
		void createPipe();
		void spawnChild();
		void IODup();
		void dupFd(int oldfd, int newfd);
		void createArgs();
		void readCGI();

		std::string getBodyFromCGI() const;

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
		int pipe_child2parent_[2];
		int pipe_parent2child_[2];
		//pid_t child_pid_;
		
};

#endif
