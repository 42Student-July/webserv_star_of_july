#include <string>
#include <vector>
#include <map>

struct LocationConfig
{
	std::string uri_;

	// Define a HTTP redirection.
	std::string root_;

	// Define a list of accepted HTTP methods for the route
	std::vector< std::string > allowed_methods_;

	// Set a default file to answer if the request is a directory.
	std::vector< std::string > indexes_;

	// Turn on or off directory listing.
	bool autoindex_;

	// Execute CGI based on certain file extension (for example .php).
	std::string cgi_path_;

};

struct ServerConfig
{
	// Choose the port and host of each ’server’.
	size_t port_;
	std::string host_;

	// Setup the server_names or not.
	std::vector<std::string> names_; 

	// Setup default error pages.
	std::map<int, std::string> error_pages_;

	// Setup routes with one or multiple of the following rules/configuration (routes wont be using regexp)
	std::string root_;

	// Limit client body size.
	size_t client_body_size_limit_;

	std::vector< LocationConfig > locations_;
};

