#ifndef WEBSERVCONFING_HPP
#define WEBSERVCONFING_HPP

#include <set>
#include <stdexcept>

#include "ServerConfig.hpp"

typedef std::set<size_t> PortSet;
typedef std::vector<ServerConfig> ServConfVector;

class WebservConfig {
 public:
  WebservConfig();  // ConfigParserのコンストラクタで必要
  WebservConfig(const ServConfVector& serv_confs);
  ~WebservConfig();
  WebservConfig(const WebservConfig& other);
  WebservConfig& operator=(const WebservConfig& other);
  const ServerConfig& findServerConfig(size_t port,
                                       const std::string& host) const;
  const PortSet& ports() const;

 private:
  const ServerConfig& findDefaultServerConfig(size_t port) const;
  ServConfVector serv_confs_;
  PortSet ports_;
};

#endif /* WEBSERVCONFING_HPP */
