#include "HttpRequestConverter.hpp"

HttpRequestConverter::HttpRequestConverter() {}

HttpRequestConverter::~HttpRequestConverter() {}

std::string HttpRequestConverter::searchRequestHeaderField(
    const HttpRequest::HeaderFieldMap& name_value_map,
    const std::string& name) {
  std::string header_field_value;

  HttpRequest::HeaderFieldMap::const_iterator it = name_value_map.find(name);
  if (it != name_value_map.end()) {
    header_field_value = it->second;
  } else {
    header_field_value = "";
  }
  return header_field_value;
}

HttpRequestDTO* HttpRequestConverter::toDTO(HttpRequest* request) {
  HttpRequestDTO* dto = new HttpRequestDTO;

  dto->method = request->request_line.method;
  dto->path = request->request_line.uri;
  dto->version = request->request_line.version.substr(sizeof("HTTP/") - 1);
  dto->body = request->body;

  dto->connection =
      searchRequestHeaderField(request->name_value_map, "Connection");
  dto->authorization =
      searchRequestHeaderField(request->name_value_map, "Authorization");
  dto->content_length =
      searchRequestHeaderField(request->name_value_map, "Content-Length");
  dto->content_type =
      searchRequestHeaderField(request->name_value_map, "Content-Type");
  dto->accept = searchRequestHeaderField(request->name_value_map, "Accept");
  dto->forwarded =
      searchRequestHeaderField(request->name_value_map, "Forwarded");
  dto->referer = searchRequestHeaderField(request->name_value_map, "Referer");
  dto->user_agent =
      searchRequestHeaderField(request->name_value_map, "User_Agent");
  dto->x_forwarded_for =
      searchRequestHeaderField(request->name_value_map, "X_Forwarded_For");

  dto->host = request->server_config.host;
  dto->port = request->server_config.port;
  dto->servernames = request->server_config.server;

  dto->response_status_code = request->response_status_code;
  return dto;
}
