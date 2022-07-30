#include "WebservConfig.hpp"

WebservConfig::WebservConfig() {}

WebservConfig::WebservConfig(const ServConfVector& serv_confs)
    : serv_confs_(serv_confs) {
  for (ServConfVector::const_iterator it = serv_confs_.begin();
       it != serv_confs_.end(); ++it) {
    ports_.insert(it->port);
  }
}

WebservConfig::~WebservConfig() {}

WebservConfig::WebservConfig(const WebservConfig& other) { *this = other; }

WebservConfig& WebservConfig::operator=(const WebservConfig& other) {
  if (this != &other) {
    serv_confs_ = other.serv_confs_;
    ports_ = other.ports_;
  }
  return *this;
}

const PortSet& WebservConfig::ports() const { return ports_; }

const ServerConfig& WebservConfig::findDefaultServerConfig(size_t port) const {
  for (ServConfVector::const_iterator it = serv_confs_.begin();
       it != serv_confs_.end(); ++it) {
    if (it->port == port) {
      return *it;
    }
  }
  throw std::runtime_error("findDefaultServerConfig: port not found");
  return serv_confs_[0];
}

const ServerConfig& WebservConfig::findServerConfig(
    size_t port, const std::string& host) const {
  const ServerConfig& default_server = findDefaultServerConfig(port);
  for (ServConfVector::const_iterator it = serv_confs_.begin();
       it != serv_confs_.end(); ++it) {
    if (it->port == port && it->host == host) {
      return *it;
    }
  }
  return default_server;
}
