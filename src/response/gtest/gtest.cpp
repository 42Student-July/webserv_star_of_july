#include <gtest/gtest.h>

#include <stdexcept>

#include "../response.h"
#include "../CGI.hpp"
#include "../CGIParser.hpp"

int set(ConfigDTO &conf_, LocationConfig &loc_demo, HttpRequestDTO &req)
{
	//conf
	conf_.root = "html";
	conf_.port = "5000";
	loc_demo.location = "/";
	loc_demo.root = "/html";
	loc_demo.allowed_methods.push_back("GET");
	loc_demo.autoindex = true;
	loc_demo.indexes.push_back("index.html");
	loc_demo.cgi_path = "/cgi-bin";
	conf_.locations.push_back(loc_demo);

	
	// http request data
	req.method = "GET";
	req.path = "/";
	req.file = "test.py";
	req.body = "aaaaaaaaaaaa";

	//builder
	/* HttpResponseBuilder builder = HttpResponseBuilder(conf_); */
	/* HttpResponse *res = builder.build(req); */
	/* HttpResponseSerializer serializer = HttpResponseSerializer(); */
	/* HttpResponsePlainText *plain_text = serializer.serialize(*res); */
	/* std::cout << plain_text->Text() << std::endl; */
	return 0;
}

TEST(CGITests, CanRunCGI)
{
	ConfigDTO conf_;
	LocationConfig loc_demo;
	HttpRequestDTO req;
	set(conf_, loc_demo, req);
	CGI cgi(req, conf_);
	cgi.run();
}

TEST(CGITests, CanGetBodyFromCGI)
{
	ConfigDTO conf_;
	LocationConfig loc_demo;
	HttpRequestDTO req;
	set(conf_, loc_demo, req);
	CGI cgi(req, conf_);
	cgi.run();
	std::string body = cgi.getBodyFromCGI();
}

TEST(CGIParserTests, CanParse)
{
	ConfigDTO conf_;
	LocationConfig loc_demo;
	HttpRequestDTO req;
	set(conf_, loc_demo, req);
	CGI cgi(req, conf_);
	cgi.run();
	std::string body = cgi.getBodyFromCGI();
	CGIParser cgi_parser(body);
}
