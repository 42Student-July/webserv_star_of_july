#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <string>
#include <vector>
#include <map>

struct LocationConfig {
  std::string location;

  // Define a HTTP redirection.
  std::string root;

  // Define a list of accepted HTTP methods for the route
  std::vector<std::string> allowed_methods;

  // Set a default file to answer if the request is a directory.
  std::vector<std::string> indexes;

  // Turn on or off directory listing.
  bool autoindex;

  // Execute CGI based on certain file extension (for example .php).
  std::string cgi_path;
};

struct ConfigDTO
{
	std::map<int, std::string> error_pages;
	std::string root;
	std::string server;
	std::vector<LocationConfig> locations;
};

#endif