#ifndef CONFIG_SERVERCONFIG_HPP_
#define CONFIG_SERVERCONFIG_HPP_

#include <map>
#include <string>
#include <vector>

struct LocationConfig {
  // locationだとlocations_.location_になってしまうので一旦name_にしています
  std::string location;

  // Define a HTTP redirection.
  std::map<int, std::string> redirect;

  std::string root;

  // Define a list of accepted HTTP methods for the route
  std::vector<std::string> allowed_methods;

  // Set a default file to answer if the request is a directory.
  std::vector<std::string> indexes;

  // Turn on or off directory listing.
  bool autoindex;

  // Execute CGI based on certain file extension (for example .php).
  std::vector<std::string> cgi_extensions;
};

struct ServerConfig {
  // Choose the port and host of each ’server’.
  // necessary
  size_t port;
  std::string host;

  // Setup the server_names or not.
  std::vector<std::string> server;

  // Setup default error pages.
  std::map<int, std::string> error_pages;

  // Setup routes with one or multiple of the following rules/configuration
  // (routes wont be using regexp)
  std::string root;

  // Limit client body size.
  size_t client_body_size_limit;

  std::vector<LocationConfig> locations;
};

#endif  // CONFIG_SERVERCONFIG_HPP_
