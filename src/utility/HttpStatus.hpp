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
static const std::string OK = "200";
static const std::string CREATED = "201";
static const std::string NO_CONTENT = "204";
static const std::string PARTIAL_CONTENT = "206";
static const std::string BAD_REQUEST = "400";
static const std::string UNAUTHORIZED = "401";
static const std::string FORBIDDEN = "403";
static const std::string NOT_FOUND = "404";
static const std::string PAYLOAD_TOO_LARGE = "413";
static const std::string REQUEST_HEADER_FIELD_TOO_LARGE = "431";
static const std::string INTERNAL_SERVER_ERROR = "500";
static const std::string SERVICE_UNAVAILABLE = "503";
static const std::string HTTP_VERSION_NOT_SUPPORTED = "505";
namespace ReasonPhrase {
static const std::string OK = "OK";
static const std::string CREATED = "Created";
static const std::string NO_CONTENT = "No Content";
static const std::string PARTIAL_CONTENT = "Partial Content";
static const std::string BAD_REQUEST = "Bad Request";
static const std::string UNAUTHORIZED = "Unauthorized";
static const std::string FORBIDDEN = "Forbidden";
static const std::string NOT_FOUND = "Not Found";
static const std::string INTERNAL_SERVER_ERROR = "Internal Server Error";
static const std::string SERVICE_UNAVAILABLE = "Service Unavailable";
}  // namespace ReasonPhrase
}  // namespace HttpStatus

#endif
