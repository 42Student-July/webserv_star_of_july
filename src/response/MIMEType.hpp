#ifndef MIMETYPE_HPP
# define MIMETYPE_HPP

#include <map>
#include <string>

class MIMEType
{
private:
	typedef std::pair<std::string, std::string> mime_extension_pair_;
	std::map<std::string, std::string> mime_type_;
public:
	MIMEType();
	~MIMEType();
	MIMEType(const MIMEType &other);
	MIMEType &operator=(const MIMEType &other);
};

#endif
