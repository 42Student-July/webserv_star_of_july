#ifndef HTTPRESPONSEHEADERDTO_HPP
# define HTTPRESPONSEHEADERDTO_HPP

#include <string>

typedef struct s_header_dto
{
	std::string version;
	std::string http_status;
	std::string server;
	std::string date;
	std::string content_type;
	std::string content_length;
	std::string last_modified;
	std::string connection;
	std::string etag;
	std::string accept_ranges;
} t_header_dto;

#endif
