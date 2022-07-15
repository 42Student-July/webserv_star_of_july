#include "response.h"

int main(int argc, char const *argv[]) {
  (void)argc;
  (void)argv;
  // conf
  ConfigDTO conf_;
  conf_.root = "html";
  LocationConfig loc_demo;
  loc_demo.location = "/";
  loc_demo.root = "/html";
  loc_demo.allowed_methods.push_back("GET");
  loc_demo.autoindex = true;
  loc_demo.indexes.push_back("index.html");
  loc_demo.cgi_path = std::string();
  conf_.locations.push_back(loc_demo);

  // http request data
  HttpRequestDTO req;
  req.method = "GET";
  // req.path = "/index.html";
  req.path = "/";
  req.file = "index.html";
  req.connection = "Keep-Alive";

  // builder
  HttpResponseBuilder builder = HttpResponseBuilder(conf_);
  HttpResponse *res = builder.build(req);
  HttpResponseSerializer serializer = HttpResponseSerializer();
  HttpResponsePlainText *plain_text = serializer.serialize(*res);
  std::cout << plain_text->Text() << std::endl;
  return 0;
}
