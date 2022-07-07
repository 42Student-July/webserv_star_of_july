#include "HttpResponseBuilder.hpp"

const std::string HttpResponseBuilder::CRLF = "\r\n";

HttpResponseBuilder::HttpResponseBuilder()
{
}

HttpResponseBuilder::HttpResponseBuilder(ConfigDTO conf)
{
	conf_ = conf;
	t_abspath.exists = false;
}

HttpResponseBuilder::~HttpResponseBuilder()
{
}
HttpResponseBuilder::HttpResponseBuilder(const HttpResponseBuilder &other)
{
	*this = other;
}
HttpResponseBuilder &HttpResponseBuilder::operator=(const HttpResponseBuilder &other)
{
	if (this != &other)
	{
	}
	return *this;
}

// 基本的に文字列操作はmallocを使いたくないのでstringに変換して行いたい
void HttpResponseBuilder::findAbsPath(std::string dir, std::string file)
{
	DIR				*dirp;
	struct dirent	*ent;
	char			*cwd;
	std::string		fullpath;
	
	cwd = getcwd(NULL, 0);
	
	fullpath = std::string(cwd) + dir;
	std::free(cwd);
	std::cout << "fullpath: " << fullpath << std::endl;
	
	
	dirp = opendir(fullpath.c_str());
	if (dirp == NULL)
		throw std::runtime_error("directory not found");
	while ((ent = readdir(dirp)) != NULL)
	{
		if (std::strcmp(ent->d_name,file.c_str()) == 0)
		{
			std::cout << "kita" << std::endl;
			t_abspath.filepath = fullpath + file;
			t_abspath.exists = true;
			break;
		}
	}
	closedir(dirp);
}

void HttpResponseBuilder::findFilepath(HttpRequestData &req)
{
	std::vector<LocationConfig>::iterator i = conf_.GetLocation().begin();
	std::vector<LocationConfig>::iterator ie = conf_.GetLocation().end();

	for (; i != ie; i++)
	{
		if ((*i).location_ == req.GetPathDir())
		{
			findAbsPath((*i).root_ + (*i).location_, req.GetPathFile());
		}
	}
	if (!t_abspath.exists)
		throw std::runtime_error("file not found");
}

void HttpResponseBuilder::readFile()
{
	std::ifstream ifs(t_abspath.filepath);
	std::string line;
	
	if (ifs.fail())
		throw std::ios_base::failure("file input error");
    while (std::getline(ifs, line)){
        file_str_ << line << CRLF;
    }
}

void HttpResponseBuilder::buildHeader()
{
	header_.version = "1.1";
	header_.status_code = "200";
	header_.reason_phrase = "OK";
	header_.server = "webserv";
	header_.date = "Tue, 05 Jul 2022 06:44:07 GMT";
	header_.content_type = "text/html";
	header_.content_length = "0";
	header_.last_modified = "Mon, 04 Jul 2022 07:57:09 GMT";
	header_.connection = "keep-alive";
	header_.etag = "\"62c29d55-e5\"";
	header_.accept_ranges = "bytes";
}

HttpResponse *HttpResponseBuilder::build(HttpRequestData &req)
{
	try
	{
		findFilepath(req);
		readFile();
		buildHeader();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		// TODO:直す
		std::exit(1);
	}
	
	return new HttpResponse(header_, file_str_.str());
}
