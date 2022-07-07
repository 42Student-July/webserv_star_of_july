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
	HttpResponseHeaderDTO header,
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

const std::string &HttpResponse::Version() const
{
	return version_;
}
const std::string &HttpResponse::StatusCode() const
{
	return status_code_;
}
const std::string &HttpResponse::ReasonPhrase() const
{
	return reason_phrase_;
}
const std::string &HttpResponse::Server() const {
	return server_;
}
const std::string &HttpResponse::Date() const
{
	return date_;
}
const std::string &HttpResponse::ContentType() const
{
	return content_type_;
}
const std::string &HttpResponse::ContentLength() const
{
	return content_length_;
}
const std::string &HttpResponse::LastModified() const
{
	return last_modified_;
}
const std::string &HttpResponse::Connection() const
{
	return connection_;
}
const std::string &HttpResponse::ETag() const
{
	return etag_;
}
const std::string &HttpResponse::AcceptRanges() const
{
	return accept_ranges_;
}

const std::string &HttpResponse::Body() const
{
	return body_;
}
