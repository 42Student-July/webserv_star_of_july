#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

#include <string>
#include <sstream>
#include <iostream>
class HttpResponse
{
private:
	// 後ほど各項目に分ける
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

