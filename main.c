#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

const int MAX_EVENTS = 10;

static int listen_socket(int listen_port) {
  struct sockaddr_in addr;
  int lfd;

  lfd = socket(AF_INET, SOCK_STREAM, 0);
  if (lfd == -1) {
    perror("socket");
    return -1;
  }x

  memset(&addr, 0, sizeof(addr));
  addr.sin_port = htons(listen_port);
  addr.sin_family = AF_INET;
  if (bind(lfd, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
    perror("bind");
    close(lfd);
    return -1;
  }

  printf("accepting connections on port %d\n", listen_port);
  listen(lfd, 10);
  return lfd;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    dprintf(2, "Usage: ./select_practice <port>\n");
    return 1;
  }

  int h = listen_socket(argv[1]);
  if (h == -1)
    exit(EXIT_FAILURE);

  while (1) {
    int nfds = 0;
    ssize_t nbytes;
    fd_set readfds, writefds;

    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    nfds = max(nfds, h);

    ready = select(nfds + 1, &readfds, &writefds, NULL, NULL);

    if (ready == -1)
      exit(EXIT_FAILURE);

    for (int fd = 0; fd < nfds + 1; ++fd) {
      if (FD_ISSET(h, &readfds))
	// recv
	;
    }
  }



  return 0;
}
