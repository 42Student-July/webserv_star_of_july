#ifndef CLIENT_HPP
# define CLIENT_HPP


#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "utils.hpp"


int ClientSocket(char *hostnm, char *portnm);

std::string SendRecv(int soc, const std::string &request);

#endif
