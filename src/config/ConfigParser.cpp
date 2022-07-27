#include "ConfigParser.hpp"
#include <stdexcept>
#include "ServerConfig.hpp"

const unsigned int ConfigParser::BIT_FLAG_LISTEN = (1 << 0);  // 0000 0000 0000 0001
const unsigned int ConfigParser::BIT_FLAG_ROOT = (1 << 1);  // 0000 0000 0000 0010
const unsigned int ConfigParser::BIT_FLAG_BODY_LIMIT = (1 << 2); // 0000 0000 0000 0100
const unsigned int ConfigParser::BIT_FLAG_LOC_ROOT = (1 << 3);  // 0000 0000 0001 0000
const unsigned int ConfigParser::BIT_FLAG_AUTOINDEX = (1 << 4); // 0000 0000 0010 0000

// 定数のセット
const std::vector<std::string> ConfigParser::VALID_METHODS = ConfigParser::setValidMethods();
std::vector<std::string> ConfigParser::setValidMethods() {
	std::vector<std::string> valid_methods;

	valid_methods.push_back("GET");
	valid_methods.push_back("POST");
	valid_methods.push_back("DELETE");

	return valid_methods;
}

ConfigParser::ConfigParser() {}
ConfigParser::~ConfigParser() {}

// parseやvalidateをして最終configをgetできるように
std::vector<ServerConfig> ConfigParser::getServerConfigs() const {
  return (serverconfigs_);
}

// parseしたいconfigファイルの受け渡し
ConfigParser::ConfigParser(std::string file) {
  std::string file_content = readFile(file);

  //文字列を全てスペースで分割してトークン化
  std::vector<std::string> tokens = isspaceSplit(file_content);

  parseTokens(tokens);
}

// スペースで分割されたトークンをparse
void ConfigParser::parseTokens(std::vector<std::string> tokens) {
  std::vector<std::string>::iterator it = tokens.begin();
  std::vector<std::string>::iterator ite = tokens.end();
  for (; it < tokens.end(); it++) {
    if (*it == "server" && *(++it) == "{") {
      ServerConfig server;

	  //サーバーごとに必須項目が設定されているか確認するためのフラグ
    unsigned int exist_flag = 0;

    parseServer(server, ++it, ite, exist_flag);
	  serverValidate(server, exist_flag);

    serverconfigs_.push_back(server);

    } else {
      throw std::runtime_error("Error: Config: Wrong syntax");
    }
  }
}

void ConfigParser::parseServer(ServerConfig &server,
                               std::vector<std::string>::iterator &it,
                               std::vector<std::string>::iterator &ite,
                               unsigned int &exist_flag) {
  bool finished_with_bracket = false;
  for (; it != ite; ++it) {
    if (*it == "listen") {
      parseListen(server, ++it, exist_flag);
    } else if (*it == "server_name") {
      parseServerName(server, ++it);
    } else if (*it == "root") {
      parseRoot(server, ++it, exist_flag);
    } else if (*it == "error_page") {
      parseErrorPages(server, ++it);
    } else if (*it == "client_body_size_limit") {
      parseClientBodySizeLimit(server, ++it, exist_flag);
    } else if (*it == "location") {
	  
      LocationConfig location;
      parseLocation(location, ++it, ite);
	  locationValidate(location);

      server.locations.push_back(location);

    } else if (*it == "}") {
      finished_with_bracket = true;
      break;
    } else {
      throw std::runtime_error("Error: Config: Wrong syntax");
    }
  }
  if (finished_with_bracket == false) {
    throw std::runtime_error("Error: Config: Need to be closed by brackets");
  }
}

void ConfigParser::parseLocation(LocationConfig &location,
                                 std::vector<std::string>::iterator &it,
                                 std::vector<std::string>::iterator &ite) {
  unsigned int l_exist_flag = 0;
  location.location = *it;
  if (*(++it) == "{") {
	it++;
    for (; it != ite; ++it) {
      if (*it == "root") {
        parseLocationRoot(location, ++it, l_exist_flag);
      } else if (*it == "allowed_method") {
        parseLocationAllowedMethods(location, ++it);
      } else if (*it == "index") {
        parseLocationIndexes(location, ++it);
      } else if (*it == "autoindex") {
        parseLocationAutoindexes(location, ++it, l_exist_flag);
      } else if (*it == "cgi_extension") {
        parseLocationCGIExtension(location, ++it);
      } else if (*it == "return") {
        parseLocationRedirect(location, ++it);
      } else if (*it == "}") {
        break;
      } else {
      	throw std::runtime_error("Error: Config: Syntax error.");
      }
    }
  }
}

void ConfigParser::serverValidate(const ServerConfig &server, const int &exist_flag) {
  // 必須項目が設定されていなかったらエラー
  if (!(exist_flag & BIT_FLAG_LISTEN)) {
	throw std::runtime_error("Error: Config: Need listen setting");
  }

  if (isDupLocation(server)) {
	throw std::runtime_error("Error: Config: Duplicated location");
  }

  if (!isValidStatus(server.error_pages)) {
	  throw std::runtime_error("Error: Config: Invalid error_page");
  }
}

void ConfigParser::locationValidate(const LocationConfig &location) {
	if (!isValidVector(location.allowed_methods, VALID_METHODS)) {
		throw std::runtime_error("Error: Config: Invalid allowed_method");
	}
	if (!isValidStatus(location.redirect)) {
		throw std::runtime_error("Error: Config: Invalid return");
	}
}

// parseServer関連関数
void ConfigParser::parseListen(ServerConfig &server,
                               std::vector<std::string>::iterator &it,
                               unsigned int &exist_flag) {
  // listenが複数あったら弾く
  if (exist_flag & BIT_FLAG_LISTEN) {
    throw std::runtime_error("Error: Config: Duplicated listen");
  }
  exist_flag |= BIT_FLAG_LISTEN;

  if (it->find(":") != std::string::npos) {
    server.host = it->substr(0, it->find(":"));
    server.port = ft_stoi(it->substr(it->find(":") + 1, it->find(";")));
  } else {
    server.host = it->substr(0, it->find(":"));
    server.port = ft_stoi(it->substr(it->find(":") + 1, it->find(";")));
  }
}

void ConfigParser::parseServerName(ServerConfig &server,
                                   std::vector<std::string>::iterator &it) {
  int num = countContents(it);
  for (int i = 0; i < num; ++i, ++it) {
    if (it->find(";") != std::string::npos) {
      server.server.push_back(it->substr(0, it->find(";")));
    } else {
      server.server.push_back(*it);
    }
  }
  it--;
}

void ConfigParser::parseRoot(ServerConfig &server,
                             std::vector<std::string>::iterator &it, unsigned int &exist_flag) {
  if (exist_flag & BIT_FLAG_ROOT) {
	  throw std::runtime_error("Error: Config: Duplicated root");
  }
  exist_flag |= BIT_FLAG_ROOT;
  server.root = it->substr(0, it->find(";"));
}

void ConfigParser::parseErrorPages(ServerConfig &server,
                                   std::vector<std::string>::iterator &it) {
  int status_code = ft_stoi(*it++);
  std::string path = (*it).substr(0, it->find(";"));

  server.error_pages.insert(std::pair<int, std::string>(status_code, path));
}

void ConfigParser::parseClientBodySizeLimit(
    ServerConfig &server, std::vector<std::string>::iterator &it, unsigned int &exist_flag) {
  if (exist_flag & BIT_FLAG_BODY_LIMIT) {
	  throw std::runtime_error("Error: Config: Duplicated client_body_size_limit");
  }
  exist_flag |= BIT_FLAG_BODY_LIMIT;
  server.client_body_size_limit = ft_stoi(*it);
}

// parseLocation関連関数
void ConfigParser::parseLocationRoot(LocationConfig &location,
                                     std::vector<std::string>::iterator &it, unsigned int &l_exist_flag) {
  if (l_exist_flag & BIT_FLAG_LOC_ROOT) {
	  throw std::runtime_error("Error: Config: Duplicated root in location");
  }
  l_exist_flag |= BIT_FLAG_LOC_ROOT;
  location.root = it->substr(0, it->find(";"));
}

void ConfigParser::parseLocationAllowedMethods(
    LocationConfig &location, std::vector<std::string>::iterator &it) {
  int num = countContents(it);
  for (int i = 0; i < num; ++i, ++it) {
    if (it->find(";") != std::string::npos) {
      location.allowed_methods.push_back(it->substr(0, it->find(";")));
    } else {
      location.allowed_methods.push_back(*it);
    }
  }
  it--;
}

void ConfigParser::parseLocationIndexes(
    LocationConfig &location, std::vector<std::string>::iterator &it) {
  int num = countContents(it);
  for (int i = 0; i < num; ++i, ++it) {
    if (it->find(";") != std::string::npos) {
      location.indexes.push_back(it->substr(0, it->find(";")));
    } else {
      location.indexes.push_back(*it);
    }
  }
  it--;
}

void ConfigParser::parseLocationAutoindexes(
  LocationConfig &location, std::vector<std::string>::iterator &it, unsigned int &l_exist_flag) {
  if (l_exist_flag & BIT_FLAG_AUTOINDEX) {
	  throw std::runtime_error("Error: Config: Duplicated autoindex");
  }
  l_exist_flag |= BIT_FLAG_AUTOINDEX;
  
  if (it->substr(0, it->find(";")) == "on") {
    location.autoindex = true;
  } else if (it->substr(0, it->find(";")) == "off") {
    location.autoindex = false;
  } else {
    throw std::runtime_error("Error: Config: Invalid autoindex");
  }
}

void ConfigParser::parseLocationCGIExtension(
  LocationConfig &location, std::vector<std::string>::iterator &it) {
  int num = countContents(it);
  for (int i = 0; i < num; ++i, ++it) {
    if (it->find(";") != std::string::npos) {
      location.cgi_extensions.push_back(it->substr(0, it->find(";")));
    } else {
      location.cgi_extensions.push_back(*it);
    }
  }
  it--;
}

void ConfigParser::parseLocationRedirect(
    LocationConfig &location, std::vector<std::string>::iterator &it) {
  int status_code = ft_stoi(*it++);
  std::string str = (*it).substr(0, it->find(";"));

  location.redirect.insert(std::pair<int, std::string>(status_code, str));
}

// validate関連の関数
//このやり方で良いのか要検討
//it_x++の部分
bool ConfigParser::isDupLocation(const ServerConfig &server) {
	std::vector<LocationConfig>::const_iterator it_x = server.locations.begin();
	std::vector<LocationConfig>::const_iterator it_y = server.locations.begin();
	it_x++;
	for (;it_y != server.locations.end(); it_y++) {
		for (; it_x != server.locations.end(); it_x++) {
			if (it_x->location == it_y->location) {
				return true;
			}
		}
	}
	return false;
}

bool ConfigParser::isValidStatus(const std::map<int, std::string> &config_map) {
	std::map<int, std::string>::const_iterator it = config_map.begin();
	for (; it != config_map.end(); ++it) {
		if (it->first <= 100 || it->first >=600) {
			return false;
		}
	}
	return true;
}

bool ConfigParser::isValidVector(const std::vector<std::string> vec_to_check, const std::vector<std::string> valid_vec) {
  return std::includes(valid_vec.begin(), valid_vec.end(), vec_to_check.begin(),                                                       
                        vec_to_check.end());
}

// その他utils
int ConfigParser::countContents(std::vector<std::string>::iterator it) {
  int num = 1;
  for (; it->find(";") == std::string::npos; ++it) {
    num++;
  }
  return num;
}

std::string ConfigParser::readFile(std::string const file) {
  std::stringstream ss;
  std::string buf;
  std::ifstream ifs(file.c_str());
  if (ifs.fail()) {
    throw std::runtime_error("Error: Config: Cannot read config file");
  }
  while (getline(ifs, buf)) {
    ss << buf << "\n";
  }
  return ss.str();
}

std::vector<std::string> ConfigParser::isspaceSplit(std::string const str) {
  std::vector<std::string> tokens;
  size_t i = 0;
  size_t j = 0;

  while (str[i]) {
    if (std::isspace(str[i])) {
      if (i == j) {
        ++j;
      } else {
        tokens.push_back(std::string(str, j, i - j));
        j = i + 1;
      }
    }
    i++;
  }
  if (i != j) {
    tokens.push_back(std::string(str, j, i - j));
  }
  return tokens;
}

int ConfigParser::ft_stoi(std::string const &str) { return atoi(str.c_str()); }
