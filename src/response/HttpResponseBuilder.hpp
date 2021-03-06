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
#include "CGI.hpp"
#include "Path.hpp"
#include "MIMEType.hpp"
#include "CGIParser.hpp"

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
	struct ErrorFilepath
	{
		std::string path;
		bool		exists;
	} errorFilepath_;
	std::stringstream res_body_str_;
	std::string path_dir_;
	std::string path_file_;
	std::string default_root_;
	
	std::vector<LocationConfig>::iterator loc_it_;
	std::vector<LocationConfig>::iterator loc_ite_;
	LocationConfig found_location_;
	bool is_file_cgi;
	bool is_delete;
	static const std::string CRLF;
	static const std::string ACCEPT_RANGES;
	static const std::string OCTET_STREAM;
	static const std::string TEXT_HTML;
	static const std::string SP;
	static const std::string SLASH;
	static const int DOT_SIZE;

	time_t now_;

	CGI cgi_;
	CGIParser cgi_parser_;
	void setDefaultRoot();
	std::string getActualRoot(LocationConfig location);
	std::string getCurrentPath();
	std::string getContentTypeByExtension();
	bool isAllowedMethod(HttpRequestDTO &req);
	void deleteFile(std::string filepath);
	bool isFileReadable();
public:
	HttpResponseBuilder();
	bool IsFileCGI();
	HttpResponseBuilder(ConfigDTO conf);
	// ~HttpResponseBuilder();
	HttpResponseBuilder(const HttpResponseBuilder &other);
	HttpResponseBuilder &operator=(const HttpResponseBuilder &other);
	HttpResponse *build(HttpRequestDTO &req);
	void findFileInServer();
	void findActualFilepath(std::string dir, std::string file);
	void readFile(std::string fullpath);
	void readErrorFile(std::string fullpath);
	void buildHeader(HttpRequestDTO &req);
	void updateHeader();
	void buildErrorHeader(HttpRequestDTO &req, int httpStatus, std::string body_str);
	void findIndexFilepath(std::string dir, LocationConfig location);
	std::string buildDate();
	std::string buildLastModified();
	void parseRequestPath(const Path &path);
	void checkOptions(HttpRequestDTO &req);
	bool isCGI(std::string file);
	void doCGI(HttpRequestDTO &req);
	std::string getReasonPhrase(std::string httpStatus);
	HttpResponse *buildDefaultErrorPage(int httpstatus, HttpRequestDTO &req);
	void buildDefaultErrorBody(int httpStatus);
	HttpResponse *buildErrorResponse(int httpstatus, HttpRequestDTO &req);
	class ResponseException : public std::runtime_error {
	private:
		int http_status_;
	public:
		ResponseException(const char *_msg, int http_status);
		const int &GetHttpStatus() const;
	};

};

#endif
