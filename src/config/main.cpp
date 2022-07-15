#include "ConfigParser.hpp"

/* template <typename T> */
/* void print(T t, std::string name) { */
/* 	std::cout << name << ": "<< t << std::endl; */
/* } */

/* void printServerConfig(std::vector<ServerConfig> serverconfig) { */
/* 	std::vector<ServerConfig>::iterator it = serverconfig.begin(); */
/* 	for (; it < serverconfig.end(); it++) { */
/* 		print(it->port, "port"); */
/* 		//print(it->host); */
/* 		//print(it->name); */
/* 		print(it->root, "root"); */
/* 		print(it->locations[0].uri_, "locations[0].uri_"); */
/* 	} */
/* } */

/* int main(int ac, char *av[]) { */
/* 	if (ac != 2) { */
/* 		std::cerr << "Usage: ./webserv <config file>" << std::endl; */
/* 		return 0; */
/* 	} */
/* 	try { */
/* 		ConfigParser configparser(av[1]); */
/* 		std::vector<ServerConfig> serverconfigs =
 * configparser.getServerConfigs(); */
/* 		printServerConfig(serverconfigs); */
/* 		/1* std::cout << serverconfigs[0].name[2] << std::endl; *1/ */
/* 		/1* std::cout << serverconfigs[0].root << std::endl; *1/ */
/* 		/1* std::cout << serverconfigs[0].locations[0].uri_ <<
 * std::endl; *1/ */

/* 	} catch (...) { */
/* 		std::cout << "something is wrong" << std::endl; */

/* 	} */
/* } */
