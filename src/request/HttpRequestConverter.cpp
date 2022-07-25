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
  const HeaderFieldMap& headers = req.name_value_map;

  dto->method = req.request_line.method;
  dto->path = req.request_line.uri;
  dto->version = req.request_line.version.substr(sizeof("HTTP/") - 1);
  dto->body = req.body;

  dto->connection = searchFieldValue(headers, "connection");
  dto->authorization = searchFieldValue(headers, "authorization");
  dto->content_length = searchFieldValue(headers, "content-length");
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
