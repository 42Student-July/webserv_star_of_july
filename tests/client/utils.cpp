#include "utils.hpp"

std::string cutResponse(std::string response)
{
	std::string shorten_response;
	std::stringstream ss(response);
	std::string line;
	while (std::getline(ss, line, '\n')) {
		if (line.find("Date:") != std::string::npos ||
			line.find("ETag:") != std::string::npos) {
			continue;
		} else {
			shorten_response += line;
			shorten_response += "\n";
		}
	}
	return shorten_response;
}

void writeFile(std::string file_name, std::string file_contents) {
	std::ofstream writing_file;
	writing_file.open(file_name, std::ios_base::out);
	if (!writing_file.is_open())
		std::cerr << "fail to open" << file_name << std::endl;
	writing_file << file_contents << std::endl;
	writing_file.close();
}

std::string readFile(std::string const file) {
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
