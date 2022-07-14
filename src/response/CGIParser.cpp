#include "CGIParser.hpp"

CGIParser::CGIParser(std::string cgi_response)
	:cgi_response_(cgi_response)
{
}

CGIParser::~CGIParser()
{
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
