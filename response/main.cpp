#include "response.h"

int main(int argc, char const *argv[])
{
	(void)argc;
	(void)argv;
	//conf
	std::string location = "/";
	std::string root = "html";
	std::string index = "index.html";
	ConfigDTO conf = ConfigDTO();
	conf.buildDemoConf();
	
	// http request data
	std::string method = "GET";
	std::string path = "/";
	std::string file = "index.html";
	bool is_index = true;
	HttpRequestData data = HttpRequestData(method, path, file, is_index);

	//builder
	HttpResponseBuilder builder = HttpResponseBuilder(conf);
	HttpResponse *res = builder.build(data);
	res->printResponse();
	return 0;
}
