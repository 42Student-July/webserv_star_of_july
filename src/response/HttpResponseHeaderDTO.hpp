#ifndef HTTPRESPONSEHEADERDTO_HPP
#define HTTPRESPONSEHEADERDTO_HPP

#include <string>

struct HttpResponseHeaderDTO {
  std::string version;
  std::string status_code;
  std::string reason_phrase;
  std::string server;
  std::string date;
  std::string content_type;
  std::string content_length;
  std::string last_modified;
  std::string connection;
  std::string etag;
  std::string accept_ranges;
};

#endif
