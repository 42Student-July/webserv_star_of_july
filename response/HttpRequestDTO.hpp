#ifndef HTTPREQUESTDTO_HPP
# define HTTPREQUESTDTO_HPP

#include <string>

struct HttpRequestDTO
{
	std::string method;
	std::string path;
	std::string version;
	std::string connection;
	std::string body;
	
	// TODO: 後で消す
	std::string file;
};


#endif
