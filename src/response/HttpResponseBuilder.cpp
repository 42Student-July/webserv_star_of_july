#include "HttpResponseBuilder.hpp"

const std::string HttpResponseBuilder::CRLF = "\r\n";
const std::string HttpResponseBuilder::ACCEPT_RANGES = "none";
const std::string HttpResponseBuilder::OCTET_STREAM =
    "application/octet-stream";

HttpResponseBuilder::HttpResponseBuilder() {}

HttpResponseBuilder::HttpResponseBuilder(ConfigDTO conf) {
  conf_ = conf;
  filepath_.exists = false;
  // builder初期化時に現在時刻を更新
  time(&now_);
  loc_it_ = conf_.locations.begin();
  loc_ite_ = conf_.locations.end();
}

HttpResponseBuilder::HttpResponseBuilder(ConfigDTO conf)
{
	conf_ = conf;
	filepath_.exists = false;
	is_file_cgi = false;
	// builder初期化時に現在時刻を更新
	time(&now_);
	loc_it_ = conf_.locations.begin();
	loc_ite_ = conf_.locations.end();
}
HttpResponseBuilder &HttpResponseBuilder::operator=(
    const HttpResponseBuilder &other) {
  if (this != &other) {
  }
  return *this;
}

bool HttpResponseBuilder::isCGI(std::string file)
{
	size_t extension_pos;
	std::vector<std::string>::iterator it = found_location_.cgi_extensions.begin();
	std::vector<std::string>::iterator ite = found_location_.cgi_extensions.end();
	for (; it != ite; it++)
	{
		extension_pos = file.find(*it);
		// TODO:拡張子のみのファイル名の場合の処理を検討
		if (extension_pos != std::string::npos) {
			std::cout << "is cgi true" << std::endl;
			return true;
		}
	}
	return false;
}

// 基本的に文字列操作はmallocを使いたくないのでstringに変換して行いたい
void HttpResponseBuilder::findActualFilepath(std::string dir, std::string file)
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
		if (std::strcmp(ent->d_name, file.c_str()) == 0)
		{
			filepath_.path = fullpath + file;
			filepath_.exists = true;
			break;
		}
	}
	closedir(dirp);
}

void HttpResponseBuilder::findIndexFilepath(LocationConfig location)
{
	if (location.indexes.size() == 0)
	{
		// indexが存在しない場合はindex.htmlがデフォルトで入る
		file_ = "index.html";
		findActualFilepath(location.root + location.location, file_);
		return;
	}
	std::vector<std::string>::iterator it = location.indexes.begin();
	std::vector<std::string>::iterator ite = location.indexes.end();
	for (; it != ite; it++)
	{
		findActualFilepath(location.root + location.location, *it);
		if (filepath_.exists)
		{
			file_ = *it;
			break;
		}
	}
}

void HttpResponseBuilder::findFileInServer()
{
	// TODO: 定義が重複しているときのアルゴリズム作成
	for (; loc_it_ != loc_ite_; loc_it_++)
	{
		if ((*loc_it_).location == dir_)
		{
			if (file_.empty())
				findIndexFilepath(*loc_it_);
			else
				findActualFilepath((*loc_it_).root + (*loc_it_).location, file_);
			if (filepath_.exists)
			{
				found_location_ = *loc_it_;
				break;
			}
		}
	}
	if (!filepath_.exists)
		throw std::runtime_error("file not found");
}

void HttpResponseBuilder::readFile(std::string fullpath)
{
	std::ifstream ifs(fullpath.c_str());
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
	date.erase(date.size() - 1);
	date += " GMT";
	return date;
}

std::string HttpResponseBuilder::buildLastModified()
{
	std::string mod_time;
	struct stat s;
	time_t time;
	
	if(stat(filepath_.path.c_str(), &s) == -1)
	{
		std::runtime_error("stat");
	}
	time = s.st_mtime;
	mod_time = asctime(gmtime(&time));
	mod_time.erase(mod_time.size() - 1);
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
	header_.content_type = OCTET_STREAM;
	header_.content_length = file_str_.str().size();
	header_.last_modified = buildLastModified();
	header_.connection = req.connection;
	// 特にこういうふうにしろみたいな指定があるわけでもなさそう RFC7232
	// なので固定文字列をおいてみる
	header_.etag = "\"62c29d55-e5\"";
	// noneで固定
	header_.accept_ranges = ACCEPT_RANGES;
}

void HttpResponseBuilder::reflectLocationStatus()
{
	if (isCGI(file_))
		is_file_cgi = true;
	//TODO: allowed_methodとかはこっちにおく
}

void HttpResponseBuilder::doCGI()
{
	//TODO: ここにCGIの処理を追加
}
HttpResponse *HttpResponseBuilder::buildErrorResponse(int httpstatus)
{
	std::string error_page = conf_.error_pages[httpstatus];
	if (error_page.empty())
		return buildDefaultErrorPage(httpstatus);
	readFile()	
}

HttpResponse *HttpResponseBuilder::build(HttpRequestDTO &req)
{
	try
	{
		parseRequestPath(req.path);
		findFileInServer();
		reflectLocationStatus();
		if (is_file_cgi)
		{
			doCGI();
			// ここにcgi用のbuidlerとかを配置
		}
		else
		{
			readFile(filepath_.path);
			buildHeader(req);
		}
	}
	catch(const ResponseException re)
	{
		std::cout << "re.GetHttpStatus: " << re.GetHttpStatus() << std::endl;
		return buildErrorResponse(re.GetHttpStatus());
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		// TODO:直す
		std::exit(1);
	}
	return new HttpResponse(header_, file_str_.str());
}

HttpResponseBuilder::ResponseException::ResponseException(const char *_msg, int http_status) : runtime_error(_msg)
{	
	http_status_ = http_status;
}

const int &HttpResponseBuilder::ResponseException::GetHttpStatus() const
{
	return http_status_;
}
