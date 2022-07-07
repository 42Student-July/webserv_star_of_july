#include "HttpResponseSerializer.hpp"

const std::string HttpResponseSerializer::PROTOCOL = "HTTP/";
const std::string HttpResponseSerializer::SERVER = "Server: ";
const std::string HttpResponseSerializer::DATE = "Date: ";
const std::string HttpResponseSerializer::CONTENT_TYPE = "Content-Type: ";
const std::string HttpResponseSerializer::CONTENT_LENGTH = "Content-Length: ";
const std::string HttpResponseSerializer::LAST_MODIFIED = "Last-Modified: ";
const std::string HttpResponseSerializer::ETAG = "ETag: ";
const std::string HttpResponseSerializer::ACCEPT_RANGES = "Accept-Ranges: ";

HttpResponseSerializer::HttpResponseSerializer()
{
}
HttpResponseSerializer::~HttpResponseSerializer()
{
}
HttpResponseSerializer::HttpResponseSerializer(const HttpResponseSerializer &other)
{
	*this = other;
}
HttpResponseSerializer &HttpResponseSerializer::operator=(const HttpResponseSerializer &other)
{
	if (this != &other)
	{
	}
	return *this;
}

HttpResponsePlainText *HttpResponseSerializer::serialize(HttpResponse res)
{
	
}
