#ifndef UTILS_HPP
# define UTILS_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::string cutResponse(std::string response);

void writeFile(std::string file_name, std::string file_contents);

std::string readFile(std::string const file);

#endif

