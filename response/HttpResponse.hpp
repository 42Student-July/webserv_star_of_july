#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

#include <string>
#include <sstream>
#include <iostream>
class HttpResponse
{
private:
	// header attr
	std::string version_;
	std::string http_status_;
	std::string server_;
	std::string date_;
	std::string content_type_;
	std::string content_length_;
	std::string last_modified_;
	std::string connection_;
	std::string etag_;
	std::string accept_ranges_;

	std::string header_;
	std::string body_;
	std::size_t body_size_;
	std::size_t total_size_;
public:
	HttpResponse();
	HttpResponse(
		std::string header, 
		std::string body, 
		std::size_t body_size, 
		std::size_t total_size
	);
	
	~HttpResponse();
	HttpResponse(const HttpResponse &other);
	HttpResponse &operator=(const HttpResponse &other);
	void printResponse();
};

#endif

