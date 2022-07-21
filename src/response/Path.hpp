#ifndef PATH_HPP
#define PATH_HPP

#include <iostream>
#include <vector>

#include "ConfigDTO.hpp"

class Path {
 public:
  Path(const std::string &pathquery, const ConfigDTO &config);
  virtual ~Path();
  //使わない
  Path(Path const &other);
  Path &operator=(Path const &other);

  const std::string &getRawPath() const;
  std::string getQuery() const;
  std::vector<std::string> getArgs() const;
  std::string getExtension() const;
  std::vector<std::string> getVecPath() const;

  const std::string &getPathDir() const;
  const std::string &getPathFile() const;

  void splitPathQuery();
  void parseRequestPath(std::string req_path);
  void createArgs();
  void findLocalPath();
  void setExtension();

  // utilsに入れる
  std::vector<std::string> splitStr(std::string str, std::string sep);

 private:
  const ConfigDTO &conf_;
  const std::string &pathquery_;

  std::string path_dir_;
  std::string path_file_;
  std::string raw_path_;
  std::vector<std::string> vec_path_;
  std::string query_;
  std::vector<std::string> args_;
  std::string extension_;
};

#endif
