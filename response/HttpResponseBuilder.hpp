#ifndef HTTPRESPONSEBUILDER_HPP
# define HTTPRESPONSEBUILDER_HPP

#include <vector>
#include <iostream>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <dirent.h>
#include <fstream>
#include <ios>
#include <cstring>
#include <time.h>

#include "ConfigDTO.hpp"
#include "HttpResponse.hpp"
#include "HttpResponseHeaderDTO.hpp"
#include "HttpRequestDTO.hpp"
#include "HttpStatus.hpp"


class HttpResponseBuilder
{
private:
	ConfigDTO conf_;
	HttpResponseHeaderDTO header_;
	struct s_abspath
	{
		std::string filepath;
		bool		exists;
	} t_abspath;
	std::stringstream file_str_;
	static const std::string CRLF;
	time_t now_;
public:
	HttpResponseBuilder();
	HttpResponseBuilder(ConfigDTO conf);
	~HttpResponseBuilder();
	HttpResponseBuilder(const HttpResponseBuilder &other);
	HttpResponseBuilder &operator=(const HttpResponseBuilder &other);
	HttpResponse *build(HttpRequestDTO &req);
	void findFilepath(HttpRequestDTO &req);
	void findAbsPath(std::string dir, std::string file);
	void readFile();
	void buildHeader(HttpRequestDTO &req);
	std::string buildNow();
};

#endif
