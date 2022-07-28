#ifndef HTTPREQUESTCONVERTER_HPP
#define HTTPREQUESTCONVERTER_HPP

#include "HttpRequest.hpp"
#include "HttpRequestDTO.hpp"

class HttpRequestConverter {
 public:
  HttpRequestConverter();
  ~HttpRequestConverter();
  HttpRequestDTO* toDTO(const HttpRequest& request);

 private:
  HttpRequestConverter(const HttpRequestConverter& other);
  HttpRequestConverter& operator=(const HttpRequestConverter& other);
  static std::string searchFieldValue(const HeaderFieldMap& name_value_map,
                                      const std::string& name);
};

#endif /* HTTPREQUESTCONVERTER_HPP */
