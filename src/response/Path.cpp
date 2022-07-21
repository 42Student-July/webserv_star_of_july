#include "Path.hpp"

Path::Path(const std::string &pathquery, const ConfigDTO &conf)
    : conf_(conf), pathquery_(pathquery) {
  splitPathQuery();
  createArgs();
  vec_path_ = splitStr(raw_path_, "/");
  //とりあえずここで一つ消してる
  vec_path_.erase(vec_path_.begin());

  parseRequestPath(raw_path_);
  setExtension();
}

Path::~Path() {}

// いるかどうか検討
Path::Path(Path const &other)
    : conf_(other.conf_), pathquery_(other.pathquery_) {
  *this = other;
}

Path &Path::operator=(Path const &other) {
  if (this != &other) {
  }
  return *this;
}

const std::string &Path::getRawPath() const { return raw_path_; }

const std::string &Path::getPathDir() const { return path_dir_; }

const std::string &Path::getPathFile() const { return path_file_; }

std::string Path::getQuery() const { return query_; }

std::vector<std::string> Path::getArgs() const { return args_; }

std::vector<std::string> Path::getVecPath() const { return vec_path_; }

std::string Path::getExtension() const { return extension_; }

void Path::parseRequestPath(std::string req_path) {
  size_t last_slash_pos = req_path.find_last_of('/');
  if (last_slash_pos == std::string::npos) {
    throw std::runtime_error("no slash found in request path");
  }
  path_dir_ = req_path.substr(0, last_slash_pos + 1);
  path_file_ = req_path.substr(last_slash_pos + 1);
}

void Path::splitPathQuery() {
  size_t query_pos = pathquery_.find('?');
  if (query_pos == std::string::npos) {
    raw_path_ = pathquery_;
    query_ = "";
    return;
  }
  raw_path_ = pathquery_.substr(0, query_pos);
  query_ = pathquery_.substr(query_pos + 1);
}

// utilsに入れる
// 1から入るようになってしまってる
std::vector<std::string> Path::splitStr(std::string str, std::string sep) {
  std::vector<std::string> ret;
  size_t start_pos = 0;
  size_t split_pos = str.find(sep);
  for (; split_pos != std::string::npos;) {
    ret.push_back(str.substr(start_pos, split_pos - start_pos));
    start_pos = split_pos + sep.size();
    split_pos = str.find(sep, start_pos);
  }
  ret.push_back(str.substr(start_pos));

  return ret;
}

void Path::createArgs() {
  if (query_.find('=') != std::string::npos) {
    return;
  }
  args_ = splitStr(query_, "+");
}

void Path::setExtension() {
  /* std::vector<std::string>::iterator it = vec_path_.begin(); */
  /* for (;it != vec_path_.end(); ++it) { */
  /* } */

  //修正する
  extension_ = ".pl";
}

// todo: localhost/test.cgi/arg1/arg2の時の対処検討
