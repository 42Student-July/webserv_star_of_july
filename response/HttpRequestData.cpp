#include "HttpRequestData.hpp"

HttpRequestData::HttpRequestData(){}

HttpRequestData::HttpRequestData(std::string method, std::string path_dir, std::string path_file, bool is_index)
{
	method_ = method;
	path_dir_ = path_dir;
	path_file_ = path_file;
	is_index_ = is_index;
}

HttpRequestData::~HttpRequestData()
{
}
HttpRequestData::HttpRequestData(const HttpRequestData &other)
{
	*this = other;
}
HttpRequestData &HttpRequestData::operator=(const HttpRequestData &other)
{
	if (this != &other)
	{
	}
	return *this;
}

const std::string &HttpRequestData::GetPathDir() const
{
	return path_dir_;
}

const std::string &HttpRequestData::GetPathFile() const
{
	return path_file_;
}
