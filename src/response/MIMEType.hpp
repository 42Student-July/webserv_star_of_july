#ifndef MIMETYPE_HPP
# define MIMETYPE_HPP

#include <map>
#include <string>

typedef std::pair<std::string, std::string> MimeTypePair;


class MIMEType
{
private:
	std::map<std::string, std::string> mime_type_map_;
	std::string default_;
	static const std::string OCTET_STREAM;
	MimeTypePair p(const char *extension, const char *content_type);
public:
	MIMEType();
	~MIMEType();
	MIMEType(const MIMEType &other);
	MIMEType &operator=(const MIMEType &other);
	const std::string &Default() const;
	const std::map<std::string, std::string> &GetMap() const;
};

#endif
