#ifndef HTTPRESPONSEPLAINTEXT_HPP
# define HTTPRESPONSEPLAINTEXT_HPP

#include <string>

class HttpResponsePlainText
{
private:
	std::string text;
	std::size_t size;
public:
	HttpResponsePlainText();
	~HttpResponsePlainText();
	HttpResponsePlainText(const HttpResponsePlainText &other);
	HttpResponsePlainText &operator=(const HttpResponsePlainText &other);
};

#endif

