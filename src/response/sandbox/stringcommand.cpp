#include <string>
#include <iostream>


void parse(std::string path)
{
	std::string dir;
	std::string file;
	
	int last_slash = path.find_last_of("/");
	dir = path.substr(0, last_slash + 1);
	file = path.substr(last_slash + 1);
	std::cout << "dir: " << dir << std::endl;
	std::cout << "file: " << file << std::endl;
}

int main(void)
{
	std::string path = "/hello/path/index.html";
	std::string no_dir = "/trend";
	std::string index = "/";
	std::string no_file = "/hello/world/";
	// parse(no_dir);
	// parse(index);
	parse(no_file);
}
