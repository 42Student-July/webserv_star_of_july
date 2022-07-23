#ifndef REQUESTLINE_HPP
#define REQUESTLINE_HPP

struct RequestLine {
  std::string method;
  std::string uri;
  std::string version;
};

#endif /* REQUESTLINE_HPP */
