#include <string>
#include <iostream>

int main(void)
{
	std::string dir;
	std::string file;
	
	std::string path = "hello/path/index.html";
	int last_slash = path.find_last_of("/");
	dir = path.substr(0, last_slash);
	file = path.substr(last_slash + 1);
	std::cout << "dir: " << dir << std::endl;
	std::cout << "file: " << file << std::endl;
}
