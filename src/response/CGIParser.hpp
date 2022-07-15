#ifndef CGIPARSER_HPP
# define CGIPARSER_HPP

#include "HttpResponseBuilder.hpp"

#include <iostream>

class CGIParser
{
    public:
        CGIParser(std::string cgi_response);
        virtual ~CGIParser();
		
		void run();
		void parseHeader();

        CGIParser(CGIParser const &other);
        CGIParser &operator=(CGIParser const &other);

    private:
		std::string cgi_response_;

		//std::map<std::string, std::string> header_;
		std::stringstream file_str_;
};

#endif
