#include "CGIParser.hpp"

CGIParser::CGIParser(std::string cgi_response)
	:cgi_response_(cgi_response)
{
}

CGIParser::~CGIParser()
{
}

void CGIParser::run()
{
	parseHeader();
	
}

void CGIParser::parseHeader()
{
	for (;;) {
		size_t end = 0;
		end = cgi_response_.find("\r\n");
		std::string header_line = cgi_response_.substr(0, end);
		std::cout << header_line << std::endl;
	}

}

CGIParser::CGIParser(CGIParser const &other)
{
    *this = other;
}

CGIParser &CGIParser::operator=(CGIParser const &other)
{
    if (this != &other)
    {
    }
    return *this;
}
