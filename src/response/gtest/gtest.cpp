#include <gtest/gtest.h>

#include <stdexcept>
#include <fstream>
#include <iostream>


#include "../response.h"
#include "../CGI.hpp"
#include "../CGIParser.hpp"
#include "../Path.hpp"

int setGet(ConfigDTO &conf_, LocationConfig &loc_demo, HttpRequestDTO &req)
{
	//conf
	conf_.root = "html";
	conf_.port = "5000";
	loc_demo.location = "/";
	loc_demo.root = "/html";
	loc_demo.allowed_methods.push_back("GET");
	loc_demo.autoindex = true;
	loc_demo.indexes.push_back("index.html");
	loc_demo.cgi_extensions.push_back("/cgi-bin");
	conf_.locations.push_back(loc_demo);

	// http request data
	req.method = "GET";
	req.path = "/cgi-bin/test.py";
	req.body = "";

	return 0;
}

int setPost(ConfigDTO &conf_, LocationConfig &loc_demo, HttpRequestDTO &req)
{
	//conf
	conf_.root = "html";
	conf_.port = "5000";
	loc_demo.location = "/";
	loc_demo.root = "/html";
	loc_demo.allowed_methods.push_back("GET");
	loc_demo.allowed_methods.push_back("POST");
	loc_demo.autoindex = true;
	loc_demo.indexes.push_back("index.html");
	loc_demo.cgi_extensions.push_back("/cgi-bin");
	conf_.locations.push_back(loc_demo);
	
	// http request data
	req.method = "POST";
	req.path = "/cgi-bin/test.py";
	req.body = "a=abc&b=bcd";

	return 0;
}

std::string readFile(std::string const file) {
  std::stringstream ss;
  std::string buf;
  std::ifstream ifs(file.c_str());
  if (ifs.fail()) {
    throw std::runtime_error("Error: Config: Cannot read config file");
  }
  while (getline(ifs, buf)) {
    ss << buf << "\n";
  }
  return ss.str();
}

TEST(PathTests, CanGetRawPath)
{
	ConfigDTO conf;
	LocationConfig loc_demo;
	HttpRequestDTO req;
	setGet(conf, loc_demo, req);

	Path path("/index.html", conf);
	ASSERT_EQ("/index.html", path.getRawPath());

	Path path_with_query("/test.py?user=yuki", conf);
	ASSERT_EQ("/test.py", path_with_query.getRawPath());
}

TEST(PathTests, CanGetQuery)
{
	ConfigDTO conf;
	LocationConfig loc_demo;
	HttpRequestDTO req;
	setGet(conf, loc_demo, req);

	Path path("/test.py?user=yuki", conf);
	ASSERT_EQ("user=yuki", path.getQuery());

	Path path_dual_query("/test.py?user=yuki&pass=password", conf);
	ASSERT_EQ("user=yuki&pass=password", path_dual_query.getQuery());
}

TEST(PathTests, CanGetArgs)
{
	ConfigDTO conf;
	LocationConfig loc_demo;
	HttpRequestDTO req;
	setGet(conf, loc_demo, req);

	Path path("/test.py?arg1+arg2", conf);
	ASSERT_EQ("arg1", path.getArgs()[0]);
	ASSERT_EQ("arg2", path.getArgs()[1]);
}

TEST(PathTests, CanSplitPath)
{
	ConfigDTO conf;
	LocationConfig loc_demo;
	HttpRequestDTO req;
	setGet(conf, loc_demo, req);

	Path path("/test.py/arg1/arg2", conf);
	ASSERT_EQ("test.py", path.getVecPath()[0]);
	ASSERT_EQ("arg1", path.getVecPath()[1]);
	ASSERT_EQ("arg2", path.getVecPath()[2]);
}

TEST(PathTests, CanGetExtension)
{
	ConfigDTO conf;
	LocationConfig loc_demo;
	HttpRequestDTO req;
	setGet(conf, loc_demo, req);

	Path path("/cgi-bin/test.py/arg1/arg2", conf);
	ASSERT_EQ(".py", path.getExtension());
}

TEST(CGITests, CanRunCGI)
{
	ConfigDTO conf;
	LocationConfig loc_demo;
	HttpRequestDTO req;
	setGet(conf, loc_demo, req);

	Path path(req.path, conf);
	CGI cgi;
	cgi.run(req, conf, path);
}

TEST(CGITests, CanGetResponseFromCGI)
{
	ConfigDTO conf;
	LocationConfig loc_demo;
	HttpRequestDTO req;
	setGet(conf, loc_demo, req);

	Path path(req.path, conf);
	CGI cgi;

	cgi.run(req, conf, path);
	std::string res_cgi = cgi.getResponseFromCGI();

	ASSERT_EQ(readFile("cgi-bin/testpy_res.txt"), res_cgi);
}

TEST(CGIParserTests, CanParseHeader)
{
	ConfigDTO conf;
	LocationConfig loc_demo;
	HttpRequestDTO req;
	setGet(conf, loc_demo, req);

	Path path(req.path, conf);
	CGI cgi;
	CGIParser cgi_parser;

	cgi.run(req, conf, path);
	std::string res_cgi = cgi.getResponseFromCGI();

	cgi_parser.parse(res_cgi);
	ASSERT_EQ("text/html", cgi_parser.getCGIHeader()["Content-type"]);
	//ASSERT_EQ("http://127.0.0.1/", cgi_parser.getCGIHeader()["Location"]);
	//ASSERT_EQ("200", cgi_parser.getCGIHeader()["StatusCode"]);
}

/* TEST(CGITests, CanPOST) */
/* { */
/* 	ConfigDTO conf_; */
/* 	LocationConfig loc_demo; */
/* 	HttpRequestDTO req; */
/* 	setPost(conf_, loc_demo, req); */
/* 	CGI cgi; */
/* 	cgi.run(req, conf_); */
/* 	std::string cgi_response = cgi.getResponseFromCGI(); */
/* } */

/* TEST(ConnectToBuilder, CanRunCGI) */
/* { */
/* 	ConfigDTO conf_; */
/* 	LocationConfig loc_demo; */
/* 	HttpRequestDTO req; */
/* 	setGet(conf_, loc_demo, req); */

/*   // builder */
/*   HttpResponseBuilder builder = HttpResponseBuilder(conf_); */
/*   HttpResponse *res = builder.build(req); }*/
  

/* void setReqForConfTest(HttpRequestDTO &req) */
/* { */
/* 	req.method = "GET"; */
/* 	req.version = "1.1"; */
/* 	req.path = "/index.html"; */
/* 	req.connection = "Keep-Alive"; */
/* } */

/* std::string ReadIndexHtml() */
/* { */
/* 	std::ifstream ifs("./html/index.html"); */
/* 	std::string line; */
/* 	std::stringstream res; */
	
/* 	while (std::getline(ifs, line)){ */
/*         res << line << "\r\n"; */
/*     } */
/* 	return res.str(); */
/* } */

/* TEST(ConfTest, rootがlocation_directiveに先頭スラッシュで存在) */
/* { */
/* 	ConfigDTO conf_; */
/* 	LocationConfig loc; */
/* 	HttpRequestDTO req; */
/* 	setReqForConfTest(req); */
	
/* 	loc.root = "html"; */
/* 	loc.location = "/"; */
/* 	conf_.locations.push_back(loc); */

/* 	// builder */
/* 	HttpResponseBuilder builder = HttpResponseBuilder(conf_); */
/* 	HttpResponse *res = builder.build(req); */
	
/* 	EXPECT_EQ(res->Body(), ReadIndexHtml()); */
/* } */
