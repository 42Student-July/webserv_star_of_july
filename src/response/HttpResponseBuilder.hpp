#ifndef HTTPRESPONSEBUILDER_HPP
#define HTTPRESPONSEBUILDER_HPP

#include <dirent.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <ios>
#include <iostream>
#include <vector>

#include "ConfigDTO.hpp"
#include "HttpRequestDTO.hpp"
#include "HttpResponse.hpp"
#include "HttpResponseHeaderDTO.hpp"
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
	} filepath_;
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
	void readFile(std::string fullpath);
	void buildHeader(HttpRequestDTO &req);
	void findIndexFilepath(LocationConfig location);
	std::string buildDate();
	std::string buildLastModified();
	void parseRequestPath(std::string req_path);
	void reflectLocationStatus();
	bool isCGI(std::string file);
	void doCGI();
	HttpResponse *buildDefaultErrorPage(int httpstatus);
	HttpResponse *buildErrorResponse(int httpstatus);
	class ResponseException : public std::runtime_error {
	private:
		int http_status_;
	public:
		ResponseException(const char *_msg, int http_status);
		const int &GetHttpStatus() const;
	};

};

#endif
