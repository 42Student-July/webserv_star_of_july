#include "HttpRequestConverter.hpp"

HttpRequestConverter::HttpRequestConverter() {}

HttpRequestConverter::~HttpRequestConverter() {}

std::string HttpRequestConverter::searchFieldValue(
    const HeaderFieldMap& name_value_map, const std::string& name) {
  HeaderFieldMap::const_iterator it = name_value_map.find(name);

  return (it != name_value_map.end()) ? it->second : "";
}

HttpRequestDTO* HttpRequestConverter::toDTO(const HttpRequest& req) {
  HttpRequestDTO* dto = new HttpRequestDTO;
  const RequestLine& request_line = req.header.requestLine();
  const HeaderFieldMap& headers = req.header.headerMap();

  dto->method = request_line.method;
  dto->path = request_line.uri;
  if (request_line.version.find("HTTP/") != std::string::npos) {
    dto->version = request_line.version.substr(sizeof("HTTP/") - 1);
  } else {
    dto->version = "";
  }
  dto->body = req.body;

  dto->connection = searchFieldValue(headers, "connection");
  dto->authorization = searchFieldValue(headers, "authorization");
  size_t body_size = req.body.size();
  dto->content_length = utility::toString(body_size);
  dto->content_type = searchFieldValue(headers, "content-type");
  dto->accept = searchFieldValue(headers, "accept");
  dto->forwarded = searchFieldValue(headers, "forwarded");
  dto->referer = searchFieldValue(headers, "referer");
  dto->user_agent = searchFieldValue(headers, "user_agent");
  dto->x_forwarded_for = searchFieldValue(headers, "x_forwarded_for");

  dto->host = req.server_config.host;
  dto->port = req.server_config.port;
  dto->servernames = req.server_config.server;

  dto->response_status_code = req.response_status_code;
  return dto;
}
