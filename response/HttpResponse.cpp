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

HttpResponse::HttpResponse(
	t_header_dto header,
	std::string body
)
{
	version_ = header.version;
	status_code_ = header.status_code;
	reason_phrase_ = header.reason_phrase;
	server_ = header.server;
	date_ = header.date;
	content_type_ = header.content_type;
	content_length_ = header.content_length;
	last_modified_ = header.last_modified;
	connection_ = header.connection;
	etag_ = header.etag;
	accept_ranges_ = header.accept_ranges;
	body_ = body;
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
