#include "MIMEType.hpp"

MIMEType::MIMEType()
{
	// ref /usr/local/etc/nginx/mime.types
	mime_type_.insert("html", "text/html");
	mime_type_.insert("htm", "text/html");
	mime_type_.insert("shtml", "text/html");
	mime_type_.insert("css", "text/css");
	mime_type_.insert("xml", "text/xml");
	mime_type_.insert("gif", "image/gif");
	mime_type_.insert("jpeg", "image/jpeg");
	mime_type_.insert("jpg", "image/jpeg");
	mime_type_.insert("js", "application/javascript");
	mime_type_.insert("txt", "text/plain");
	mime_type_.insert("png", "image/png");
	mime_type_.insert("json", "application/json");
	mime_type_.insert("pdf", "application/pdf");
	mime_type_.insert("zip", "application/zip");
	mime_type_.insert("bin", "application/octet-stream");
	mime_type_.insert("exe", "application/octet-stream");
	mime_type_.insert("img", "application/octet-stream");
	mime_type_.insert("dmg", "application/octet-stream");
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
