#ifndef HTTPSTATUS_HPP
#define HTTPSTATUS_HPP

#include <string>

// enum HttpStatus {
//     HTTP_OK = 200,
//     HTTP_NO_CONTENT = 204,
//     HTTP_PARTIAL_CONTENT = 206,
//     HTTP_MOVED_PERMANENTLY = 301,
//     HTTP_FOUND = 302,
//     HTTP_SEE_OTHER = 303,
//     HTTP_NOT_MODIFIED = 304,
//     HTTP_TEMPORARY_REDIRECT = 307,
//     HTTP_BAD_REQUEST = 400,
//     HTTP_UNAUTHORIZED = 401,
//     HTTP_FORBIDDEN = 403,
//     HTTP_NOT_FOUND = 404,
//     HTTP_INTERNAL_SERVER_ERROR = 500,
//     HTTP_SERVICE_UNAVAILABLE = 503,
// };
namespace HttpStatus {
static std::string OK = "200";
static std::string CREATED = "201";
static std::string NO_CONTENT = "204";
static std::string PARTIAL_CONTENT = "206";
static std::string BAD_REQUEST = "400";
static std::string UNAUTHORIZED = "401";
static std::string FORBIDDEN = "403";
static std::string NOT_FOUND = "404";
static std::string INTERNAL_SERVER_ERROR = "500";
static std::string SERVICE_UNAVAILABLE = "503";
namespace ReasonPhrase {
static std::string OK = "OK";
static std::string CREATED = "Created";
static std::string NO_CONTENT = "No Content";
static std::string PARTIAL_CONTENT = "Partial Content";
static std::string BAD_REQUEST = "Bad Request";
static std::string UNAUTHORIZED = "Unauthorized";
static std::string FORBIDDEN = "Forbidden";
static std::string NOT_FOUND = "Not Found";
static std::string INTERNAL_SERVER_ERROR = "Internal Server Error";
static std::string SERVICE_UNAVAILABLE = "Service Unavailable";
}  // namespace ReasonPhrase
}  // namespace HttpStatus

#endif
