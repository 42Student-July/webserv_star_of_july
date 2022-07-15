#ifndef HTTPREQUESTCONVERTER_HPP
#define HTTPREQUESTCONVERTER_HPP

#include "HttpRequest.hpp"
#include "HttpRequestDTO.hpp"

class HttpRequestConverter {
 public:
  HttpRequestConverter();
  ~HttpRequestConverter();
  HttpRequestDTO* toDTO(HttpRequest* request);

 private:
  HttpRequestConverter(const HttpRequestConverter& other);
  HttpRequestConverter& operator=(const HttpRequestConverter& other);
  static std::string searchRequestHeaderField(
      const HttpRequest::HeaderFieldMap& name_value_map,
      const std::string& name);
};

#endif /* HTTPREQUESTCONVERTER_HPP */
