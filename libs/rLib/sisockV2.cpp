#include "sisockV2.h"

struct sockaddr *build_sin(struct sockaddr_in *sa,char *ip,int port) {
  memset(sa,0,sizeof(struct sockaddr_in));
  sa->sin_family=AF_INET;
  sa->sin_port=htons(port);
  sa->sin_addr.s_addr=(ip)?inet_addr(ip):htonl(INADDR_ANY);
  return (struct sockaddr*)sa;
}
