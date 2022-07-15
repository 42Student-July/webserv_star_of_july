#ifndef HTTPRESPONSEPLAINTEXT_HPP
#define HTTPRESPONSEPLAINTEXT_HPP

#include <string>

class HttpResponsePlainText {
 private:
  std::string text_;
  std::size_t size_;

 public:
  HttpResponsePlainText();
  HttpResponsePlainText(std::string text, std::size_t size);
  ~HttpResponsePlainText();
  HttpResponsePlainText(const HttpResponsePlainText &other);
  HttpResponsePlainText &operator=(const HttpResponsePlainText &other);
  const std::string &Text() const;
  const std::size_t &Size() const;
};

#endif
