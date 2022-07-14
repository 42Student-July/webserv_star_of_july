#ifndef CGIPARSER_HPP
# define CGIPARSER_HPP

#include <iostream>

class CGIParser
{
    public:
        CGIParser(std::string cgi_response);
        virtual ~CGIParser();
        CGIParser(CGIParser const &other);
        CGIParser &operator=(CGIParser const &other);

    private:
		std::string cgi_response_;
};

#endif
