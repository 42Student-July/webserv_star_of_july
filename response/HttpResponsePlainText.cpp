#include "HttpResponsePlainText.hpp"

HttpResponsePlainText::HttpResponsePlainText()
{
}

HttpResponsePlainText::HttpResponsePlainText(std::string text, std::size_t size)
{
	text_ = text;
	size_ = size;
}
HttpResponsePlainText::~HttpResponsePlainText()
{
}
HttpResponsePlainText::HttpResponsePlainText(const HttpResponsePlainText &other)
{
	*this = other;
}
HttpResponsePlainText &HttpResponsePlainText::operator=(const HttpResponsePlainText &other)
{
	if (this != &other)
	{
	}
	return *this;
}

const std::string &HttpResponsePlainText::Text() const
{
	return text_;
}
const std::size_t &HttpResponsePlainText::Size() const
{
	return size_;
}
