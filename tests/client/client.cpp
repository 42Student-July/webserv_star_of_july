#include "client.hpp"
#include "utils.hpp"

int ClientSocket(char *hostnm, char *portnm) {
  struct in_addr addr;
  struct hostent *host;
  struct sockaddr_in server;
  struct servent *se;
  int soc, portno;

  memset((char *)&server, 0, sizeof(server));
  server.sin_family = AF_INET;

  if ((int)(addr.s_addr = inet_addr(hostnm)) == -1) {
    host = gethostbyname(hostnm);
    if (host == NULL) {
      fprintf(stderr, "gethostbyname():error\n");
      return (-1);
    }
    memcpy(&addr, (struct in_addr *)*host->h_addr_list, sizeof(struct in_addr));
  }
  //fprintf(stderr, "addr=%s\n", inet_ntoa(addr));
  server.sin_addr = addr;

  if (isdigit(portnm[0])) {
    if ((portno = atoi(portnm)) <= 0) {
      fprintf(stderr, "bad port no\n");
      return (-1);
    }
    server.sin_port = htons(portno);
  } else {
    if ((se = getservbyname(portnm, "tcp")) == NULL) {
      fprintf(stderr, "getservbyname():error\n");
      return (-1);
    } else {
      server.sin_port = se->s_port;
    }
  }
  //fprintf(stderr, "port=%d\n", ntohs(server.sin_port));

  if ((soc = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    return (-1);
  }

  if (connect(soc, (struct sockaddr *)&server, sizeof(server)) == -1) {
    perror("connect");
    close(soc);
    return (-1);
  }

  return (soc);
}

std::string SendRecv(int soc, const T_TEST &test) {
  int len;
  char buf[5120];
  int error;

  error = 0;
     if ((len = send(soc, test.request.c_str(), test.request.size(), 0)) < 0) {
       perror("send");
       error = 1;
     }
     if ((len = recv(soc, buf, sizeof(buf), 0)) < 0) {
       perror("recv");
       error = 1;
     }
     if (len == 0) {
       fprintf(stderr, "recv:EOF\n");
       error = 1;
     }
     buf[len] = '\0';
    if (error) {
		exit(1);
    }
	std::string response(buf);

  return response;
}



int main(int argc, char *argv[]) {
  int soc;
  std::vector<T_TEST> test;

  if (argc <= 2) {
    fprintf(stderr, "client server-host port\n");
    return (1);
  }
  std::vector<T_TEST> tests = makeTests();
  std::vector<T_TEST>::const_iterator it = tests.begin();

  int test_no = 0;

  for (; it != tests.end(); it++){
	  soc = ClientSocket(argv[1], argv[2]);
	  if (soc == -1) {
		fprintf(stderr, "ClientSocket():error\n");
		return (-1);
	  }

	  test_no++;
	
	  std::string response = SendRecv(soc, *it);

	  std::string shorten_response = cutResponse(response);

	  // テストNo.をstringにする
	  std::stringstream ss; 
	  ss << test_no;

	  std::string log_file_name = "log_response/test_" + ss.str();

	  writeFile(log_file_name, shorten_response);

	  if (readFile(log_file_name) == it->expected_response) {
		  std::cout << YELLOW"TEST[" << test_no << "]: " << GREEN"[ OK ]" << RESET << std::endl;
	  } else {
		  std::cout << YELLOW"TEST[" << test_no << "]: " << RED"[ NG ]" << RESET << std::endl;
		  
		  if (it->request_file_path == "") {
			  std::cout << YELLOW"response:\n" << RESET << shorten_response << std::endl;
			  std::cout << YELLOW"expected_response:\n" << RESET << it->expected_response << std::endl;
		  } else {
			  std::string diff_string = "diff expected_response/" + it->request_file_path.substr(8) + ".res " + "log_response/test_" + ss.str();
			  std::cout << YELLOW"diff:" << RESET << std::endl; 
			  system(diff_string.c_str());
		  }
	  }

	  close(soc);
  }


  return (0);
}
