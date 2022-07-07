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
	HttpRequestDTO req;
	req.method = "GET";
	req.path = "/";
	req.file = "index.html";

	//builder
	HttpResponseBuilder builder = HttpResponseBuilder(conf);
	HttpResponse *res = builder.build(req);
	HttpResponseSerializer serializer = HttpResponseSerializer();
	HttpResponsePlainText *plain_text = serializer.serialize(*res);
	std::cout << plain_text->Text() << std::endl;
	return 0;
}
