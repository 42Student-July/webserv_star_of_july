#include "ConfigDTO.hpp"

ConfigDTO::ConfigDTO()
{
	buildDemoConf();
}

ConfigDTO::~ConfigDTO()
{
}
ConfigDTO::ConfigDTO(const ConfigDTO &other)
{
	*this = other;
}
ConfigDTO &ConfigDTO::operator=(const ConfigDTO &other)
{
	if (this != &other)
	{
	}
	return *this;
}

std::vector<LocationConfig> &ConfigDTO::GetLocation()
{
	return locations_;
}

void ConfigDTO::buildDemoConf()
{
	LocationConfig loc_demo;
	
	loc_demo.location_ = "/";
	loc_demo.root_ = "/html";
	loc_demo.allowed_methods_.push_back("GET");
	loc_demo.autoindex_ = true;
	loc_demo.indexes_.push_back("index.html");
	loc_demo.cgi_path_ = std::string();
	locations_.push_back(loc_demo);
}
