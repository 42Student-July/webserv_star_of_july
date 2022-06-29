#include "ConfigParser.hpp"

template <typename T>
void print(T t, std::string name) {
	std::cout << name << ": "<< t << std::endl;
}

void printServerConfig(std::vector<ServerConfig> serverconfig) {
	std::vector<ServerConfig>::iterator it = serverconfig.begin();
	for (; it < serverconfig.end(); it++) {
		print(it->port_, "port_");
		//print(it->host_);
		//print(it->names_);
		print(it->root_, "root_");
		print(it->locations_[0].uri_, "locations_[0].uri_");
	}
}

int main(int ac, char *av[]) {
	if (ac != 2) {
		std::cerr << "Usage: ./webserv <config file>" << std::endl;
		return 0;
	}
	try {
		ConfigParser configparser(av[1]);
		std::vector<ServerConfig> serverconfigs = configparser.getServerConfigs();
		printServerConfig(serverconfigs);
		/* std::cout << serverconfigs[0].names_[2] << std::endl; */
		/* std::cout << serverconfigs[0].root_ << std::endl; */
		/* std::cout << serverconfigs[0].locations_[0].uri_ << std::endl; */

	} catch (...) {
		std::cout << "something is wrong" << std::endl;

	}

  /* Webserv s("4242"); */

  /* s.loop(); */
  /* s.shutdown(); */
  /* s.close(); */
}
