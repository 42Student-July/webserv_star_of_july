#include "ConfigParser.hpp"

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
  // print_vector(tokens);

  parseTokens(tokens);
}

std::vector<ServerConfig> ConfigParser::getServerConfigs() const {
  return (serverconfigs_);
}

ConfigParser::~ConfigParser() {}

void ConfigParser::parseTokens(std::vector<std::string> tokens) {
  std::vector<std::string>::iterator it = tokens.begin();
  std::vector<std::string>::iterator ite = tokens.end();
  for (; it < tokens.end(); it++) {
    if (*it == "server" && *(++it) == "{") {
      ServerConfig server;
      parseServer(server, ++it, ite);
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
                               std::vector<std::string>::iterator &it) {
  if (it->find(":") != std::string::npos) {
    server.host_ = it->substr(0, it->find(":"));
    server.port_ = stoi(it->substr(it->find(":") + 1, it->find(";")));
  } else {
    server.host_ = it->substr(0, it->find(":"));
    server.port_ = stoi(it->substr(it->find(":") + 1, it->find(";")));
  }
}

void ConfigParser::parseServerName(ServerConfig &server,
                                   std::vector<std::string>::iterator &it) {
  int num = countContents(it);
  for (int i = 0; i < num; ++i, ++it) {
    if (it->find(";") != std::string::npos) {
      server.names_.push_back(it->substr(0, it->find(";")));
    } else {
      server.names_.push_back(*it);
    }
  }
  it--;
}

void ConfigParser::parseRoot(ServerConfig &server,
                             std::vector<std::string>::iterator &it) {
  server.root_ = it->substr(0, it->find(";"));
}

void ConfigParser::parseErrorPages(ServerConfig &server,
                                   std::vector<std::string>::iterator &it) {
  server.error_pages_.insert(std::pair<int, std::string>(
      stoi(*it), (*(++it)).substr(0, it->find(";"))));
}

void ConfigParser::parseClientBodySizeLimit(
    ServerConfig &server, std::vector<std::string>::iterator &it) {
  server.client_body_size_limit_ = stoi(*it);
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

// ToDo:それぞれ1回ずつしか入力できないようにする
void ConfigParser::parseLocation(LocationConfig &location,
                                 std::vector<std::string>::iterator &it,
                                 std::vector<std::string>::iterator &ite) {
  location.name_ = *it;
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

// ToDo:それぞれ1回ずつしか入力できないようにする
void ConfigParser::parseServer(ServerConfig &server,
                               std::vector<std::string>::iterator &it,
                               std::vector<std::string>::iterator &ite) {
  bool finished_with_bracket = false;
  for (; it != ite; ++it) {
    if (*it == "listen") {
      parseListen(server, ++it);
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
      server.locations_.push_back(location);
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
  std::ifstream ifs(file);
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
