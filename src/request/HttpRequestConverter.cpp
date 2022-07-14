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

  dto->method = request->method;
  dto->path = request->uri;
  dto->version = request->version;

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
  dto->body = request->body;
  dto->is_bad_request = request->is_bad_request;

  dto->host = request->server_config.host_;
  dto->port = request->server_config.port_;
  dto->servernames = request->server_config.names_;
  return dto;
}
