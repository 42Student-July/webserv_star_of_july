#include "ConfigParser.hpp"
#include "ServerConfig.hpp"

const unsigned int ConfigParser::BIT_FLAG_LISTEN = (1 << 0);  // 0000 0000 0000 0001
const unsigned int ConfigParser::BIT_FLAG_HOST = (1 << 1);  // 0000 0000 0000 0010
const unsigned int ConfigParser::BIT_FLAG_S_ROOT = (1 << 2); // 0000 0000 0000 0100
const unsigned int ConfigParser::BIT_FLAG_BODY_LIMIT = (1 << 3); // 0000 0000 0000 1000
const unsigned int ConfigParser::BIT_FLAG_ = (1 << 4);  // 0000 0000 0001 0000
const unsigned int ConfigParser::BIT_FLAG_5 = (1 << 5); // 0000 0000 0010 0000
const unsigned int ConfigParser::BIT_FLAG_6 = (1 << 6); // 0000 0000 0100 0000
const unsigned int ConfigParser::BIT_FLAG_7 = (1 << 7); // 0000 0000 1000 0000

const std::vector<std::string> ConfigParser::VALID_MOETHODS = ConfigParser::setValidMethods();

std::vector<std::string> ConfigParser::setValidMethods() {
	std::vector<std::string> valid_methods;

	valid_methods.push_back("GET");
	valid_methods.push_back("POST");
	valid_methods.push_back("DELETE");

	return valid_methods;
}

template <typename T>
void print_vector(std::vector<T> vec) {
  typename std::vector<T>::iterator it = vec.begin();
  int i = 0;
  for (; it < vec.end(); it++) {
    i++;
    std::cout << i << "] " << *it << std::endl;
  }
}

ConfigParser::ConfigParser() {}

ConfigParser::ConfigParser(std::string file) {
  std::string file_content = readFile(file);
  std::vector<std::string> tokens = isspaceSplit(file_content);

  parseTokens(tokens);
}

std::vector<ServerConfig> ConfigParser::getServerConfigs() const {
  return (serverconfigs_);
}

void ConfigParser::serverValidate(const ServerConfig &server, const int &exist_flag) {
  // 必須項目が設定されていなかったらエラー
  if (!(exist_flag & BIT_FLAG_LISTEN)) {
	throw std::runtime_error("Error: Config: Need listen setting");
  }

  if (isDupLocation(server)) {
	throw std::runtime_error("Error: Config: Duplicated location");
  }
}

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

ConfigParser::~ConfigParser() {}

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

int countContents(std::vector<std::string>::iterator it) {
  int num = 1;
  for (; it->find(";") == std::string::npos; ++it) {
    num++;
  }
  return num;
}

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
                             std::vector<std::string>::iterator &it) {
  server.root = it->substr(0, it->find(";"));
}

void ConfigParser::parseErrorPages(ServerConfig &server,
                                   std::vector<std::string>::iterator &it) {
  int status_code = ft_stoi(*it++);
  std::string path = (*it).substr(0, it->find(";"));

  server.error_pages.insert(std::pair<int, std::string>(status_code, path));
}

void ConfigParser::parseClientBodySizeLimit(
    ServerConfig &server, std::vector<std::string>::iterator &it) {
  server.client_body_size_limit = ft_stoi(*it);
}

void ConfigParser::parseLocationRoot(LocationConfig &location,
                                     std::vector<std::string>::iterator &it) {
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
    LocationConfig &location, std::vector<std::string>::iterator &it) {
  if (it->substr(0, it->find(";")) == "on") {
    location.autoindex = true;
  } else if (it->substr(0, it->find(";")) == "off") {
    location.autoindex = false;
  } else {
    throw std::runtime_error("Error: Config: Wrong syntax");
  }
}

void ConfigParser::parseLocationCGIPath(
    LocationConfig &location, std::vector<std::string>::iterator &it) {
  //後ほど修正必要
  location.cgi_extensions.push_back(it->substr(0, it->find(";")));
}

void ConfigParser::parseLocationRedirect(
    LocationConfig &location, std::vector<std::string>::iterator &it) {
  int status_code = ft_stoi(*it++);
  std::string str = (*it).substr(0, it->find(";"));

  location.redirect.insert(std::pair<int, std::string>(status_code, str));
}

// ToDo:それぞれ1回ずつしか入力できないようにする
void ConfigParser::parseLocation(LocationConfig &location,
                                 std::vector<std::string>::iterator &it,
                                 std::vector<std::string>::iterator &ite) {
  location.location = *it;
  if (*(++it) == "{") {
    for (; it != ite; ++it) {
      if (*it == "root") {
        parseLocationRoot(location, ++it);
      } else if (*it == "allowed_method") {
        parseLocationAllowedMethods(location, ++it);
      } else if (*it == "index") {
        parseLocationIndexes(location, ++it);
      } else if (*it == "autoindex") {
        parseLocationAutoindexes(location, ++it);
      } else if (*it == "cgi_path") {
        parseLocationCGIPath(location, ++it);
      } else if (*it == "return") {
        parseLocationRedirect(location, ++it);
      } else if (*it == "}") {
        break;
      }
      /* } else { */
      /* 	throw std::runtime_error("Error: Something is wrong in config
       * file."); */
      /* } */
    }
  }
}

template< class InputIt, class T >
InputIt myFind(InputIt first, InputIt last, const T& value)
{
	for (; first != last; ++first) {
		if (*first == value) {
			return first;
		}
	}
	return last;
}

bool ConfigParser::validVectorCheck(const std::vector<std::string> vec_to_check, const std::vector<std::string> valid_vec) {
	std::vector<std::string>::const_iterator it = vec_to_check.begin();
	for (; it != vec_to_check.end(); it++) {
		if (myFind(valid_vec.begin(), valid_vec.end(), *it) == valid_vec.end()) {
			return false;
		}
	}
	return true;

}

bool ConfigParser::isValidAllowedMethod(const std::vector<std::string> &allowed_methods) {
	return validVectorCheck(allowed_methods, VALID_MOETHODS);
}

void ConfigParser::locationValidate(const LocationConfig &location) {
	if (!isValidAllowedMethod(location.allowed_methods)) {
		throw std::runtime_error("Error: Config: Invalid allowed_method");
	}

}

// ToDo:それぞれ1回ずつしか入力できないようにする
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
      parseRoot(server, ++it);
    } else if (*it == "error_page") {
      parseErrorPages(server, ++it);
    } else if (*it == "client_body_size_limit") {
      parseClientBodySizeLimit(server, ++it);
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

// syamaさんのやり方参考
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
