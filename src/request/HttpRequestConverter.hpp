#ifndef HTTPREQUESTCONVERTER_HPP
#define HTTPREQUESTCONVERTER_HPP

#include "../utility/utility.hpp"
#include "HttpRequest.hpp"
#include "HttpRequestDTO.hpp"

class HttpRequestConverter {
 public:
  HttpRequestConverter();
  ~HttpRequestConverter();
  HttpRequestDTO* toDTO(const HttpRequest& request);
  static std::string searchFieldValue(const HeaderFieldMap& name_value_map,
                                      const std::string& name);

 private:
  HttpRequestConverter(const HttpRequestConverter& other);
  HttpRequestConverter& operator=(const HttpRequestConverter& other);
};

#endif /* HTTPREQUESTCONVERTER_HPP */
