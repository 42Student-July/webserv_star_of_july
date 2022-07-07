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

#include "ConfigDTO.hpp"
#include "HttpRequestData.hpp"
#include "HttpResponse.hpp"
#include "HttpResponseHeaderDTO.hpp"



class HttpResponseBuilder
{
private:
	ConfigDTO conf_;
	struct s_abspath
	{
		std::string filepath;
		bool		exists;
	} t_abspath;
	static const std::string CRLF;
	t_header_dto header_dto_;
	std::ostringstream header_;
	std::stringstream file_str_;
public:
	HttpResponseBuilder();
	HttpResponseBuilder(ConfigDTO conf);
	~HttpResponseBuilder();
	HttpResponseBuilder(const HttpResponseBuilder &other);
	HttpResponseBuilder &operator=(const HttpResponseBuilder &other);
	HttpResponse *build(HttpRequestData &req);
	void findFilepath(HttpRequestData &req);
	void findAbsPath(std::string dir, std::string file);
	void readFile();
	void buildHeader();
};

#endif
