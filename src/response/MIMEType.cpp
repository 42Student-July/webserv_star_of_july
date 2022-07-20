#include "MIMEType.hpp"


const std::string MIMEType::OCTET_STREAM = "application/octet-stream";


MimeTypePair MIMEType::p(const char *extension, const char *content_type)
{
	return std::make_pair(extension, content_type);
}

MIMEType::MIMEType()
{
	// ref /usr/local/etc/nginx/mime.types
	mime_type_map_.insert(p("html", "text/html"));
	mime_type_map_.insert(p("htm", "text/html"));
	mime_type_map_.insert(p("shtml", "text/html"));
	mime_type_map_.insert(p("css", "text/css"));
	mime_type_map_.insert(p("xml", "text/xml"));
	mime_type_map_.insert(p("gif", "image/gif"));
	mime_type_map_.insert(p("jpeg", "image/jpeg"));
	mime_type_map_.insert(p("jpg", "image/jpeg"));
	mime_type_map_.insert(p("js", "application/javascript"));
	mime_type_map_.insert(p("txt", "text/plain"));
	mime_type_map_.insert(p("png", "image/png"));
	mime_type_map_.insert(p("json", "application/json"));
	mime_type_map_.insert(p("pdf", "application/pdf"));
	mime_type_map_.insert(p("zip", "application/zip"));
	mime_type_map_.insert(p("bin", "application/octet-stream"));
	mime_type_map_.insert(p("exe", "application/octet-stream"));
	mime_type_map_.insert(p("img", "application/octet-stream"));
	mime_type_map_.insert(p("dmg", "application/octet-stream"));
	default_ = OCTET_STREAM;

}
MIMEType::~MIMEType()
{
}
MIMEType::MIMEType(const MIMEType &other)
{
	*this = other;
}
MIMEType &MIMEType::operator=(const MIMEType &other)
{
	if (this != &other)
	{
	}
	return *this;
}
const std::string &MIMEType::Default() const
{
	return default_;
}
