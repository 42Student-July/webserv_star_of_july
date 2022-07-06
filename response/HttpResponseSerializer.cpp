#include "HttpResponseSerializer.hpp"

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
