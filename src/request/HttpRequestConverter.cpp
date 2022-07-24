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
      searchRequestHeaderField(request->name_value_map, "connection");
  dto->authorization =
      searchRequestHeaderField(request->name_value_map, "authorization");
  dto->content_length =
      searchRequestHeaderField(request->name_value_map, "content-length");
  dto->content_type =
      searchRequestHeaderField(request->name_value_map, "content-type");
  dto->accept = searchRequestHeaderField(request->name_value_map, "accept");
  dto->forwarded =
      searchRequestHeaderField(request->name_value_map, "forwarded");
  dto->referer = searchRequestHeaderField(request->name_value_map, "referer");
  dto->user_agent =
      searchRequestHeaderField(request->name_value_map, "user_agent");
  dto->x_forwarded_for =
      searchRequestHeaderField(request->name_value_map, "x_forwarded_for");

  dto->host = request->server_config.host;
  dto->port = request->server_config.port;
  dto->servernames = request->server_config.server;

  dto->response_status_code = request->response_status_code;
  return dto;
}
