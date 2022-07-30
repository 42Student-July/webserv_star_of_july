#ifndef TYPE_HPP
#define TYPE_HPP

#include <map>
#include <vector>

class ClientSocket;
class ServerSocket;

typedef std::map<int, ClientSocket *> ClientSocketMap;
typedef std::map<int, ServerSocket *> ServerSocketMap;
typedef std::vector<int> FdVector;

#endif /* TYPE_HPP */
