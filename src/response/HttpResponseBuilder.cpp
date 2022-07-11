#include "HttpResponseBuilder.hpp"

const std::string HttpResponseBuilder::CRLF = "\r\n";

HttpResponseBuilder::HttpResponseBuilder()
{
}

HttpResponseBuilder::HttpResponseBuilder(ConfigDTO conf)
{
	conf_ = conf;
	filepath.exists = false;
	// builder初期化時に現在時刻を更新
	time(&now_);
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
			filepath.path = fullpath + file;
			filepath.exists = true;
			break;
		}
	}
	closedir(dirp);
}

void HttpResponseBuilder::findFilepath(HttpRequestDTO &req)
{
	std::vector<LocationConfig>::iterator i = conf_.locations.begin();
	std::vector<LocationConfig>::iterator ie = conf_.locations.end();

	for (; i != ie; i++)
	{
		if ((*i).location == req.path)
		{
			findAbsPath((*i).root + (*i).location, req.file);
		}
	}
	if (!filepath.exists)
		throw std::runtime_error("file not found");
}

void HttpResponseBuilder::readFile()
{
	std::ifstream ifs(filepath.path);
	std::string line;
	
	if (ifs.fail())
		throw std::ios_base::failure("file input error");
    while (std::getline(ifs, line)){
        file_str_ << line << CRLF;
    }
}

std::string HttpResponseBuilder::buildDate()
{
	std::string date;
	
	date = asctime(gmtime(&now_));
	// asctimeがデフォルトで改行がつく使用なので、改行を削除
	date.pop_back();
	date += " GMT";
	return date;
}

std::string HttpResponseBuilder::buildLastModified()
{
	std::string mod_time;
	struct stat s;
	time_t time;
	
	if(stat(filepath.path.c_str(), &s) == -1)
	{
		std::runtime_error("stat");
	}
	time = s.st_mtimespec.tv_sec;
	mod_time = asctime(gmtime(&time));
	mod_time.pop_back();
	mod_time += " GMT";
	return mod_time;
}

void HttpResponseBuilder::buildHeader(HttpRequestDTO &req)
{

	header_.version = req.version;
	header_.status_code = HttpStatus::OK;
	header_.reason_phrase = HttpStatus::ReasonPhrase::OK;
	header_.date = buildDate();
	header_.server = conf_.server;
	header_.content_type = "text/html";
	header_.content_length = file_str_.str().size();
	header_.last_modified = buildLastModified();
	header_.connection = req.connection;
	header_.etag = "\"62c29d55-e5\"";
	header_.accept_ranges = "bytes";
}

HttpResponse *HttpResponseBuilder::build(HttpRequestDTO &req)
{
	try
	{
		findFilepath(req);
		readFile();
		buildHeader(req);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		// TODO:直す
		std::exit(1);
	}
	
	return new HttpResponse(header_, file_str_.str());
}