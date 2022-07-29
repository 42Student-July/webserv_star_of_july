#ifndef TEST_HPP
# define TEST_HPP


#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "utils.hpp"

typedef struct S_TEST {
	std::string request_file_path = "";

	std::string request;
	std::string expected_response;
} T_TEST;

std::vector<T_TEST> makeTests();

#endif
