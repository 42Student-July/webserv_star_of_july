#include "HttpResponseBuilder.hpp"
#include "HttpRequestDTO.hpp"
#include "utility.hpp"

const std::string HttpResponseBuilder::CRLF = "\r\n";
const std::string HttpResponseBuilder::ACCEPT_RANGES = "none";
const std::string HttpResponseBuilder::OCTET_STREAM = "application/octet-stream";
const std::string HttpResponseBuilder::TEXT_HTML = "text/html";
const std::string HttpResponseBuilder::SP = " ";
const std::string HttpResponseBuilder::SLASH = "/";
const int HttpResponseBuilder::DOT_SIZE = 1;


HttpResponseBuilder::HttpResponseBuilder() {}

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
	std::cout << "fullpath: " << dir << std::endl;
	
	dirp = opendir(dir.c_str());
	if (dirp == NULL)
		throw ResponseException("directory not found", 404);
	while ((ent = readdir(dirp)) != NULL)
	{
		if (std::strcmp(ent->d_name, file.c_str()) == 0)
		{
			filepath_.path = dir + file;
			filepath_.exists = true;
			break;
		}
	}
	closedir(dirp);
}

void HttpResponseBuilder::findActualErrorFilepath(std::string dir, std::string file)
{
	DIR				*dirp;
	struct dirent	*ent;
	char			*cwd;
	std::string		fullpath;
	
	//TODO: default rootを使う
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
			errorFilepath_.path = fullpath + file;
			errorFilepath_.exists = true;
			break;
		}
	}
	closedir(dirp);
}

// TODO: path_fileはreqなので後入れしない
void HttpResponseBuilder::findIndexFilepath(std::string dir, LocationConfig location)
{
	if (location.indexes.size() == 0)
	{
		// indexが存在しない場合はindex.htmlがデフォルトで入る
		path_file_ = "index.html";
		findActualFilepath(dir, path_file_);
		return;
	}
	std::vector<std::string>::iterator it = location.indexes.begin();
	std::vector<std::string>::iterator ite = location.indexes.end();
	for (; it != ite; it++)
	{
		findActualFilepath(dir, *it);
		if (filepath_.exists)
		{
			path_file_ = *it;
			break;
		}
	}
}

std::string HttpResponseBuilder::getActualRoot(LocationConfig location)
{
	std::string current_path;
	current_path = getCurrentPath();
	
	if (location.root.empty())
		return default_root_;
	if (location.root.at(0) == '/')
		return location.root;
	return (current_path + SLASH + location.root);
}

void HttpResponseBuilder::findFileInServer()
{
	std::string root;
	std::string dir;
	
	for (; loc_it_ != loc_ite_; loc_it_++)
	{
		if ((*loc_it_).location == path_dir_)
		{
			root = getActualRoot(*loc_it_);
			dir = root + (*loc_it_).location;
			if (path_file_.empty())
				findIndexFilepath(dir, *loc_it_);
			else
				findActualFilepath(dir, path_file_);
			if (filepath_.exists)
			{
				found_location_ = *loc_it_;
				break;
			}
		}
	}
	if (!filepath_.exists)
		throw ResponseException("file not found", 404);
}

void HttpResponseBuilder::readFile(std::string fullpath)
{
	std::ifstream ifs(fullpath.c_str());
	std::string line;
	
	if (ifs.fail())
		throw ResponseException("file input error", 500);
    while (std::getline(ifs, line)){
        res_body_str_ << line << CRLF;
    }
}

void HttpResponseBuilder::readErrorFile(std::string fullpath)
{
	std::ifstream ifs(fullpath.c_str());
	std::string line;
	
	if (ifs.fail())
		throw std::runtime_error("read error");
    while (std::getline(ifs, line)){
        res_body_str_ << line << CRLF;
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
		// TODO: forbidden 403のときのハンドリングする
		throw ResponseException("stat", 500);
	}
	time = s.st_mtime;
	mod_time = asctime(gmtime(&time));
	mod_time.erase(mod_time.size() - 1);
	mod_time += " GMT";
	return mod_time;
}

std::string HttpResponseBuilder::getContentTypeByExtension()
{
	MIMEType mime;
	std::string extension;
	
	size_t last_dot_pos = path_file_.find_last_of('.');
	// もし拡張子が存在しない場合には、application/octet-streamを返す
	if (last_dot_pos == std::string::npos) {
		return std::string(mime.Default());
    }
	extension = path_file_.substr(last_dot_pos + 1);
	// ファイル名 = dot + 拡張子 → octet-stream
	if ((DOT_SIZE + extension.size()) == path_file_.size())
		return std::string(mime.Default());
	
	std::map<std::string, std::string>::const_iterator mime_itr;
	mime_itr = mime.GetMap().find(extension);
	if (mime_itr == mime.GetMap().end())
		return std::string(mime.Default());
	return mime_itr->second;
}

void HttpResponseBuilder::buildHeader(HttpRequestDTO &req)
{
	header_.version = req.version;
	header_.status_code = HttpStatus::OK;
	header_.reason_phrase = HttpStatus::ReasonPhrase::OK;
	header_.date = buildDate();
	header_.server = conf_.server;
	// header_.content_type = OCTET_STREAM;
	// とりあえずブラウザから見れるようにしました。
	header_.content_type = getContentTypeByExtension();
	size_t  content_length = res_body_str_.str().size();
	header_.content_length = utility::toString(content_length);
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
	if (isCGI(path_file_))
		is_file_cgi = true;
	//TODO: allowed_methodとかはこっちにおく
}

std::string HttpResponseBuilder::getReasonPhrase(std::string httpStatus)
{
	if (httpStatus == HttpStatus::OK)
		return HttpStatus::ReasonPhrase::OK;
	else if (httpStatus == HttpStatus::CREATED)
		return HttpStatus::ReasonPhrase::CREATED;
	else if (httpStatus == HttpStatus::NO_CONTENT)
		return HttpStatus::ReasonPhrase::NO_CONTENT;
	else if (httpStatus == HttpStatus::PARTIAL_CONTENT)
		return HttpStatus::ReasonPhrase::PARTIAL_CONTENT;
	else if (httpStatus == HttpStatus::BAD_REQUEST)
		return HttpStatus::ReasonPhrase::BAD_REQUEST;
	else if (httpStatus == HttpStatus::UNAUTHORIZED)
		return HttpStatus::ReasonPhrase::UNAUTHORIZED;
	else if (httpStatus == HttpStatus::FORBIDDEN)
		return HttpStatus::ReasonPhrase::FORBIDDEN;
	else if (httpStatus == HttpStatus::NOT_FOUND)
		return HttpStatus::ReasonPhrase::NOT_FOUND;
	else if (httpStatus == HttpStatus::INTERNAL_SERVER_ERROR)
		return HttpStatus::ReasonPhrase::INTERNAL_SERVER_ERROR;
	else if (httpStatus == HttpStatus::SERVICE_UNAVAILABLE)
		return HttpStatus::ReasonPhrase::SERVICE_UNAVAILABLE;
	else
		return "";
}

//3つのcgiからのヘッダーをいれる必要あり
void HttpResponseBuilder::updateHeader()
{
	header_.status_code = HttpStatus::OK;
	header_.content_type = TEXT_HTML;
}

void HttpResponseBuilder::doCGI(HttpRequestDTO &req)
{
	//TODO: ここにCGIの処理を追加
	Path path(req.path, conf_);
	
	cgi_.run(req, conf_, path);
	std::string cgi_response = cgi_.getResponseFromCGI();
	cgi_parser_.parse(cgi_response);
	//bodyの作成
	res_body_str_ << cgi_parser_.getBodyStr();	

	buildHeader(req);
	//TODO: このあとcgiによるupdateHeaderを実行
	updateHeader();
	
}

void HttpResponseBuilder::buildErrorHeader(HttpRequestDTO &req, int httpStatus, std::string body_str)
{
	// 発生しないはずの例外
	if (body_str.empty())
		throw std::runtime_error("body uncreated");
	header_.version = req.version;
	header_.status_code = utility::toString(httpStatus);
	header_.reason_phrase = getReasonPhrase(header_.status_code);
	header_.date = buildDate();
	header_.content_length = body_str.size();
	header_.content_type = TEXT_HTML;
	header_.connection = "keep-alive";
}

void HttpResponseBuilder::buildDefaultErrorBody(int httpStatus)
{
	std::string status_code = utility::toString(httpStatus);
	
	res_body_str_	<< "<html>" << CRLF
					<< "<head><title>" << status_code << SP << getReasonPhrase(status_code) << "</title><head>" << CRLF
					<< "<body>" << CRLF
					<< "<center><h1>" << status_code << SP << getReasonPhrase(status_code) << "</h1></center>" << CRLF
					<< "</body>" << CRLF
					<< "<hr><center>" << conf_.server << "</center>" << CRLF
					<< "</body>" << CRLF
					<< "</html>" << CRLF
					<< CRLF;
}

HttpResponse *HttpResponseBuilder::buildDefaultErrorPage(int httpStatus, HttpRequestDTO &req)
{
	buildDefaultErrorBody(httpStatus);
	buildErrorHeader(req, httpStatus, res_body_str_.str());
	return new HttpResponse(header_, res_body_str_.str());
}

// http requestのpathをdirの部分とfileの部分に分解
void HttpResponseBuilder::parseRequestPath(const Path &path)
{
	path_dir_ = path.getPathDir();
	path_file_ = path.getPathFile();
}

HttpResponse *HttpResponseBuilder::buildErrorResponse(int httpstatus, HttpRequestDTO &req)
{
	std::string error_page = conf_.error_pages[httpstatus];
	if (error_page.empty())
		return buildDefaultErrorPage(httpstatus, req);
	try
	{
		if (filepath_.exists){
			if (!found_location_.root.empty())
				readFile(found_location_.root + error_page);
		} else {
			readFile(default_root_ + error_page);
		}
		buildErrorHeader(req, httpstatus, res_body_str_.str());
	}
	catch(const std::runtime_error& e)
	{
		std::cerr << e.what() << std::endl;
		return buildDefaultErrorPage(httpstatus, req);
	}
	
	return new HttpResponse(header_, res_body_str_.str());
}

std::string HttpResponseBuilder::getCurrentPath()
{
	std::string current_path;
	char *cwd;
	
	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		throw ResponseException("getcwd", 500);
	
	current_path = std::string(cwd);
	std::free(cwd);
	return current_path;
}

void HttpResponseBuilder::setDefaultRoot()
{
	std::string current_path;
	current_path = getCurrentPath();

	if (conf_.root.empty())
	{
		default_root_ = current_path;
		return;
	}	

	// configのrootが絶対パスの場合はそのまま入れる
	if (conf_.root.at(0) == '/')
		default_root_ = conf_.root;
	else
		default_root_ = current_path + SLASH + conf_.root;
}

HttpResponse *HttpResponseBuilder::build(HttpRequestDTO &req)
{
	try
	{
		setDefaultRoot();
		Path path(req.path, conf_);
		parseRequestPath(path);
		if (req.response_status_code != HttpStatus::OK)
			return buildErrorResponse(utility::toInt(req.response_status_code), req);
		findFileInServer();
		reflectLocationStatus();
		if (is_file_cgi)
		{
			doCGI(req);
			// ここにcgi用のbuidlerとかを配置
		}
		else
		{
			readFile(filepath_.path);
			buildHeader(req);
		}
	}
	catch(const ResponseException &re)
	{
		std::cout << "re.GetHttpStatus: " << re.GetHttpStatus() << std::endl;
		return buildErrorResponse(re.GetHttpStatus(), req);
	}
	catch(const std::exception& e)
	{
		// 500を返すようにする
		std::cerr << e.what() << '\n';
		return buildErrorResponse(500, req);
	}
	return new HttpResponse(header_, res_body_str_.str());
}

HttpResponseBuilder::ResponseException::ResponseException(const char *_msg, int http_status) : runtime_error(_msg)
{	
	http_status_ = http_status;
}

const int &HttpResponseBuilder::ResponseException::GetHttpStatus() const
{
	return http_status_;
}
