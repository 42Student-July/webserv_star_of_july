#include <gtest/gtest.h>

#include <stdexcept>
#include <fstream>
#include <iostream>

#include "../response.h"
#include "../CGI.hpp"
#include "../CGIParser.hpp"
#include "../Path.hpp"

void setReqForConfTest(HttpRequestDTO &req)
{
	req.method = "GET";
	req.version = "1.1";
	req.path = "/index.html";
	req.connection = "Keep-Alive";
}

void setReqPath(HttpRequestDTO &req, std::string path)
{
	req.method = "GET";
	req.version = "1.1";
	req.path = path;
	req.connection = "Keep-Alive";
}

std::string ReadIndexHtml()
{
	std::ifstream ifs("./html/index.html");
	std::string line;
	std::stringstream res;
	
	while (std::getline(ifs, line)){
        res << line << "\r\n";
    }
	return res.str();
}

std::string getCurrentPath()
{
	char *cwd;
	
	cwd = getcwd(NULL, 0);
	return std::string(cwd);
}

void setRoot(ConfigDTO &conf_, std::string root)
{
	conf_.root = root;
	LocationConfig loc;
	loc.root = root;
	loc.location = "/";
	conf_.locations.push_back(loc);
}

TEST(RootTest, LocationIsAbsPath)
{
	ConfigDTO conf_;
	LocationConfig loc;
	HttpRequestDTO req;
	setReqForConfTest(req);
	
	loc.root = getCurrentPath() + "/html";
	loc.location = "/";
	conf_.locations.push_back(loc);

	// builder
	HttpResponseBuilder builder = HttpResponseBuilder(conf_);
	HttpResponse *res = builder.build(req);
	
	EXPECT_EQ(res->Body(), ReadIndexHtml());
}

TEST(RootTest, LocationIsRelativePath)
{
	ConfigDTO conf_;
	LocationConfig loc;
	HttpRequestDTO req;
	setReqForConfTest(req);
	
	loc.root = "html";
	loc.location = "/";
	conf_.locations.push_back(loc);

	// builder
	HttpResponseBuilder builder = HttpResponseBuilder(conf_);
	HttpResponse *res = builder.build(req);
	
	EXPECT_EQ(res->Body(), ReadIndexHtml());
}

TEST(RootTest, ServerIsAbsPath)
{
	ConfigDTO conf_;
	LocationConfig loc;
	HttpRequestDTO req;
	setReqForConfTest(req);
	
	conf_.root = getCurrentPath() + "/html";
	loc.location = "/";
	conf_.locations.push_back(loc);

	// builder
	HttpResponseBuilder builder = HttpResponseBuilder(conf_);
	HttpResponse *res = builder.build(req);
	
	EXPECT_EQ(res->Body(), ReadIndexHtml());
}

TEST(RootTest, ServerIsRelativePath)
{
	ConfigDTO conf_;
	LocationConfig loc;
	HttpRequestDTO req;
	setReqForConfTest(req);
	
	conf_.root = "html";
	loc.location = "/";
	conf_.locations.push_back(loc);

	// builder
	HttpResponseBuilder builder = HttpResponseBuilder(conf_);
	HttpResponse *res = builder.build(req);
	
	EXPECT_EQ(res->Body(), ReadIndexHtml());
}

TEST(RootTest, ServerAndLocation_locationAreAbsPath)
{
	ConfigDTO conf_;
	LocationConfig loc;
	HttpRequestDTO req;
	setReqForConfTest(req);
	
	conf_.root = "tekitou";
	loc.root = getCurrentPath() + "/html";
	loc.location = "/";
	conf_.locations.push_back(loc);

	// builder
	HttpResponseBuilder builder = HttpResponseBuilder(conf_);
	HttpResponse *res = builder.build(req);
	
	EXPECT_EQ(res->Body(), ReadIndexHtml());
}

TEST(RootTest, ServerAndLocation_locationAreRelativePath)
{
	ConfigDTO conf_;
	LocationConfig loc;
	HttpRequestDTO req;
	setReqForConfTest(req);
	
	conf_.root = "tekitou";
	loc.root = "html";
	loc.location = "/";
	conf_.locations.push_back(loc);

	// builder
	HttpResponseBuilder builder = HttpResponseBuilder(conf_);
	HttpResponse *res = builder.build(req);
	
	EXPECT_EQ(res->Body(), ReadIndexHtml());
}

TEST(MIMETest, html)
{
	ConfigDTO conf_;
	HttpRequestDTO req;
	setReqForConfTest(req);
	setRoot(conf_, std::string("html"));

	// builder
	HttpResponseBuilder builder = HttpResponseBuilder(conf_);
	HttpResponse *res = builder.build(req);
	
	EXPECT_EQ(res->ContentType(), "text/html");
}

TEST(MIMETest, htm)
{
	ConfigDTO conf_;
	LocationConfig loc;
	HttpRequestDTO req;
	setReqPath(req, std::string("/index.htm"));
	setRoot(conf_, std::string("html"));

	// builder
	HttpResponseBuilder builder = HttpResponseBuilder(conf_);
	HttpResponse *res = builder.build(req);
	
	EXPECT_EQ(res->ContentType(), "text/html");
}

TEST(MIMETest, shtml)
{
	ConfigDTO conf_;
	LocationConfig loc;
	HttpRequestDTO req;
	setReqPath(req, std::string("/index.shtml"));
	setRoot(conf_, std::string("html"));
	
	conf_.root = "html";
	loc.location = "/";
	conf_.locations.push_back(loc);

	// builder
	HttpResponseBuilder builder = HttpResponseBuilder(conf_);
	HttpResponse *res = builder.build(req);
	
	EXPECT_EQ(res->ContentType(), "text/html");
}

TEST(MIMETest, css)
{
	ConfigDTO conf_;
	LocationConfig loc;
	HttpRequestDTO req;
	setReqPath(req, std::string("/index.css"));
	setRoot(conf_, std::string("html"));
	
	conf_.root = "html";
	loc.location = "/";
	conf_.locations.push_back(loc);

	// builder
	HttpResponseBuilder builder = HttpResponseBuilder(conf_);
	HttpResponse *res = builder.build(req);
	
	EXPECT_EQ(res->ContentType(), "text/css");
}

TEST(MIMETest, xml)
{
	ConfigDTO conf_;
	LocationConfig loc;
	HttpRequestDTO req;
	setReqPath(req, std::string("/index.xml"));
	setRoot(conf_, std::string("html"));
	
	conf_.root = "html";
	loc.location = "/";
	conf_.locations.push_back(loc);

	// builder
	HttpResponseBuilder builder = HttpResponseBuilder(conf_);
	HttpResponse *res = builder.build(req);
	
	EXPECT_EQ(res->ContentType(), "text/xml");
}

TEST(MIMETest, txt)
{
	ConfigDTO conf_;
	LocationConfig loc;
	HttpRequestDTO req;
	setReqPath(req, std::string("/sample.txt"));
	setRoot(conf_, std::string("html"));
	
	conf_.root = "html";
	loc.location = "/";
	conf_.locations.push_back(loc);

	// builder
	HttpResponseBuilder builder = HttpResponseBuilder(conf_);
	HttpResponse *res = builder.build(req);
	
	EXPECT_EQ(res->ContentType(), "text/plain");
}

TEST(MIMETest, no_extension)
{
	ConfigDTO conf_;
	LocationConfig loc;
	HttpRequestDTO req;
	setReqPath(req, std::string("/sample"));
	setRoot(conf_, std::string("html"));
	
	conf_.root = "html";
	loc.location = "/";
	conf_.locations.push_back(loc);

	// builder
	HttpResponseBuilder builder = HttpResponseBuilder(conf_);
	HttpResponse *res = builder.build(req);
	
	EXPECT_EQ(res->ContentType(), "application/octet-stream");
}

TEST(MIMETest, only_dot_extension)
{
	ConfigDTO conf_;
	LocationConfig loc;
	HttpRequestDTO req;
	setReqPath(req, std::string("/.txt"));
	setRoot(conf_, std::string("html"));
	
	conf_.root = "html";
	loc.location = "/";
	conf_.locations.push_back(loc);

	// builder
	HttpResponseBuilder builder = HttpResponseBuilder(conf_);
	HttpResponse *res = builder.build(req);
	
	EXPECT_EQ(res->ContentType(), "application/octet-stream");
}

TEST(MIMETest, only_extension)
{
	ConfigDTO conf_;
	LocationConfig loc;
	HttpRequestDTO req;
	setReqPath(req, std::string("/txt"));
	setRoot(conf_, std::string("html"));
	
	conf_.root = "html";
	loc.location = "/";
	conf_.locations.push_back(loc);

	// builder
	HttpResponseBuilder builder = HttpResponseBuilder(conf_);
	HttpResponse *res = builder.build(req);
	
	EXPECT_EQ(res->ContentType(), "application/octet-stream");
}

TEST(MIMETest, end_is_dot)
{
	ConfigDTO conf_;
	LocationConfig loc;
	HttpRequestDTO req;
	setReqPath(req, std::string("/file."));
	setRoot(conf_, std::string("html"));
	
	conf_.root = "html";
	loc.location = "/";
	conf_.locations.push_back(loc);

	// builder
	HttpResponseBuilder builder = HttpResponseBuilder(conf_);
	HttpResponse *res = builder.build(req);
	
	EXPECT_EQ(res->ContentType(), "application/octet-stream");
}
