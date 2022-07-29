#include "response_maker.hpp"
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
  fprintf(stderr, "addr=%s\n", inet_ntoa(addr));
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
  fprintf(stderr, "port=%d\n", ntohs(server.sin_port));

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

std::string SendRecv(int soc, const std::string &request) {
  int len;
  char buf[5120];
  int error;


  error = 0;
     if ((len = send(soc, request.c_str(), request.size(), 0)) < 0) {
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

  if (argc <= 3) {
    fprintf(stderr, "client server-host port filepath\n");
    return (1);
  }

  soc = ClientSocket(argv[1], argv[2]);
  if (soc == -1) {
	fprintf(stderr, "ClientSocket():error\n");
	return (-1);
  }

  std::string filepath(argv[3]);
  std::string request = readFile(filepath);
  std::string response = SendRecv(soc, request);
  std::string shorten_response = cutResponse(response);

  // substrでresponse/を削除して、expected_response/ pathを追加
  std::string res_filename = "expected_response/" + filepath.substr(8) + ".res";
  close(soc);
  writeFile(res_filename, shorten_response);

  return (0);
}
