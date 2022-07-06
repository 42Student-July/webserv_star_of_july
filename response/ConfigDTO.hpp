#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <string>
#include <vector>
#include <map>

struct LocationConfig {
  std::string location_;

  // Define a HTTP redirection.
  std::string root_;

  // Define a list of accepted HTTP methods for the route
  std::vector<std::string> allowed_methods_;

  // Set a default file to answer if the request is a directory.
  std::vector<std::string> indexes_;

  // Turn on or off directory listing.
  bool autoindex_;

  // Execute CGI based on certain file extension (for example .php).
  std::string cgi_path_;
};

// DTOクラス
class ConfigDTO
{
private:
	std::map<int, std::string> error_pages_;
	std::string root_;
	std::vector<LocationConfig> locations_;
public:
	ConfigDTO();
	~ConfigDTO();
	ConfigDTO(const ConfigDTO &other);
	ConfigDTO &operator=(const ConfigDTO &other);
	void buildDemoConf();
	std::vector<LocationConfig> &GetLocation();
	// const std::string &GetRoot() const;
};

#endif

