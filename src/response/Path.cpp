#include "Path.hpp"

Path::Path(const std::string &pathquery)
	:pathquery_(pathquery)
{
	splitPath();
	createArgs();
}

Path::~Path()
{
}

Path::Path(Path const &other)
	:pathquery_(other.pathquery_)
{
    *this = other;
}

Path &Path::operator=(Path const &other)
{
    if (this != &other)
    {
    }
    return *this;
}

const std::string & Path::getRawPath() const
{
	return raw_path_;
}

std::string Path::getQuery() const
{
	return query_;
}

std::vector<std::string> Path::getArgs() const
{
	return args_;
}

void Path::splitPath()
{
	size_t query_pos = pathquery_.find('?');
	if (query_pos == std::string::npos) {
		raw_path_ = pathquery_;
		query_ = "";
		return ;
	}
	raw_path_ = pathquery_.substr(0, query_pos);
	query_ = pathquery_.substr(query_pos + 1);
}

void Path::createArgs()
{
	if (query_.find('=') != std::string::npos) {
		return ;
	}

	size_t start_pos = 0;
	size_t split_pos = query_.find('+');
	for (; split_pos != std::string::npos;) {
		args_.push_back(query_.substr(start_pos, split_pos - start_pos));
		start_pos = split_pos + 1;
		split_pos = query_.find('+', start_pos);
	}
	args_.push_back(query_.substr(start_pos));

}

//todo: localhost/test.cgi/arg1/arg2の時の対処検討
