#ifndef HTTPRESPONSEBUILDER_HPP
# define HTTPRESPONSEBUILDER_HPP

#include <vector>
#include <iostream>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fstream>
#include <ios>
#include <cstdlib>
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
	struct Filepath
	{
		std::string path;
		bool		exists;
	} filepath;
	std::stringstream file_str_;
	std::string dir_;
	std::string file_;
	
	std::vector<LocationConfig>::iterator loc_it_;
	std::vector<LocationConfig>::iterator loc_ite_;
	LocationConfig found_location_;
	bool is_file_cgi;
	static const std::string CRLF;
	static const std::string ACCEPT_RANGES;
	static const std::string OCTET_STREAM;
	time_t now_;
public:
	HttpResponseBuilder();
	HttpResponseBuilder(ConfigDTO conf);
	~HttpResponseBuilder();
	HttpResponseBuilder(const HttpResponseBuilder &other);
	HttpResponseBuilder &operator=(const HttpResponseBuilder &other);
	HttpResponse *build(HttpRequestDTO &req);
	void findFileInServer();
	void findActualFilepath(std::string dir, std::string file);
	void readFile();
	void buildHeader(HttpRequestDTO &req);
	void findIndexFilepath(LocationConfig location);
	std::string buildDate();
	std::string buildLastModified();
	void parseRequestPath(std::string req_path);
	void checkFileStatus();
	bool isCGI(std::string file);
	void doCGI();
	
};

#endif
