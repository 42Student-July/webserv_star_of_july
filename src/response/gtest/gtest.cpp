#include <gtest/gtest.h>

#include <stdexcept>

#include "../response.h"
#include "../CGI.hpp"
#include "../CGIParser.hpp"

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
	req.path = "/";
	req.file = "test.py";
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
	req.path = "/";
	req.file = "test.py";
	req.body = "a=abc&b=bcd";

	return 0;
}

TEST(CGITests, CanRunCGI)
{
	ConfigDTO conf_;
	LocationConfig loc_demo;
	HttpRequestDTO req;
	setGet(conf_, loc_demo, req);
	CGI cgi;
	cgi.run(req, conf_);
}

TEST(CGITests, CanGetBodyFromCGI)
{
	ConfigDTO conf_;
	LocationConfig loc_demo;
	HttpRequestDTO req;
	setGet(conf_, loc_demo, req);
	CGI cgi;
	cgi.run(req, conf_);
	std::string body = cgi.getResponseFromCGI();
}

TEST(CGITests, CanPOST)
{
	ConfigDTO conf_;
	LocationConfig loc_demo;
	HttpRequestDTO req;
	setPost(conf_, loc_demo, req);
	CGI cgi;
	cgi.run(req, conf_);
	std::string cgi_response = cgi.getResponseFromCGI();
}


TEST(CGIParserTests, CanParse)
{
	ConfigDTO conf_;
	LocationConfig loc_demo;
	HttpRequestDTO req;
	setGet(conf_, loc_demo, req);
	CGI cgi;
	cgi.run(req, conf_);
	std::string cgi_response = cgi.getResponseFromCGI();
	CGIParser cgi_parser;
	cgi_parser.parse(cgi_response);
}

TEST(ConnectToBuilder, CanRunCGI)
{
	ConfigDTO conf_;
	LocationConfig loc_demo;
	HttpRequestDTO req;
	setGet(conf_, loc_demo, req);

  // builder
  HttpResponseBuilder builder = HttpResponseBuilder(conf_);
  HttpResponse *res = builder.build(req);
  

}