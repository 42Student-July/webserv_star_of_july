#include "HttpResponse.hpp"

HttpResponse::HttpResponse()
{
}

HttpResponse::HttpResponse(
		std::string header, 
		std::string body, 
		std::size_t body_size, 
		std::size_t total_size
)
{
	header_ = header;
	body_ = body;
	body_size_ = body_size;
	total_size_ = total_size;
}

HttpResponse::~HttpResponse()
{
}
HttpResponse::HttpResponse(const HttpResponse &other)
{
	*this = other;
}
HttpResponse &HttpResponse::operator=(const HttpResponse &other)
{
	if (this != &other)
	{
	}
	return *this;
}

void HttpResponse::printResponse()
{
	std::cout << header_ << body_ << std::endl;
}
