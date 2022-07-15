#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <string>
#include <vector>
#include <map>
#include "../config/ServerConfig.hpp"

struct ConfigDTO
{
	std::string port;
	std::string host;
	std::map<int, std::string> error_pages;
	std::string root;
	std::string server;
	std::vector<LocationConfig> locations;
};

#endif
