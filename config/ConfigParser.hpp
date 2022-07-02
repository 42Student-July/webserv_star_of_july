#ifndef CONFIG_HPP
# define CONFIG_HPP

#include "ServerConfig.hpp"
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sstream>
#include <vector>
#include <cctype>
#include <fstream>
#include <stdexcept>

#define BUFFER_SIZE 128

// コンストラクタでparseを実行
// getServerConfigsで必要な情報(ServerConfigのベクター)を外部へ渡す
class ConfigParser
{
    public:
        ConfigParser();
        ConfigParser(std::string file);
        virtual ~ConfigParser();
        ConfigParser(ConfigParser const &other);
        ConfigParser &operator=(ConfigParser const &other);

		std::vector<ServerConfig> getServerConfigs() const;

    private:
		std::vector< ServerConfig > serverconfigs_;
		
		//void parseLocation(LocationConfig &location, std::vector<std::string>::iterator &it);
		void parseLocation(LocationConfig &location, std::vector<std::string>::iterator &it, std::vector<std::string>::iterator &ite);
		void parseLocationCGIPath(LocationConfig &location, std::vector<std::string>::iterator &it);
		void parseLocationAutoindexes(LocationConfig &location, std::vector<std::string>::iterator &it);
		void parseLocationIndexes(LocationConfig &location, std::vector<std::string>::iterator &it);
		void parseLocationRoot(LocationConfig &location, std::vector<std::string>::iterator &it);
		void parseLocationAllowedMethods(LocationConfig &location, std::vector<std::string>::iterator &it);
		void parseClientBodySizeLimit(ServerConfig &server, std::vector<std::string>::iterator &it);
		void parseErrorPages(ServerConfig &server, std::vector<std::string>::iterator &it);
		void parseRoot(ServerConfig &server, std::vector<std::string>::iterator &it);
		void parseServerName(ServerConfig &server, std::vector<std::string>::iterator &it);
		void parseListen(ServerConfig &server, std::vector<std::string>::iterator &it);
		//void parseServer(ServerConfig &server, std::vector<std::string>::iterator &it);
		void parseServer(ServerConfig &server, std::vector<std::string>::iterator &it, std::vector<std::string>::iterator &ite);
		void parseTokens(std::vector<std::string> tokens);
		std::string readFile(std::string file);
		size_t count_lines(std::string str);
		std::vector<std::string> split(const std::string &s, char delim);
		char char_after_spaces(std::string str);
		void parse_servers(std::vector<std::string> tokens);
		std::vector<std::string> isspaceSplit(std::string str);

};

#endif
