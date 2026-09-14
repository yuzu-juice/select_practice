#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int listen_socket(int listen_port) {
  struct sockaddr_in addr;
  int lfd;

  lfd = socket(AF_INET, SOCK_STREAM, 0);
  if (lfd == -1) {
    perror("socket");
    return -1;
  }

  int on = 1;
  setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

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

  int server_fd = listen_socket(atoi(argv[1]));
  if (server_fd == -1)
    exit(EXIT_FAILURE);
  int clients[FD_SETSIZE];
  for (int i = 0; i < FD_SETSIZE; ++i)
    clients[i] = -1;

  char buf[1024];

  while (1) {
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(server_fd, &readfds);
    int nfds = server_fd;
    for (int i = 0; i < FD_SETSIZE; ++i) {
      if (clients[i] == -1) continue;
      FD_SET(clients[i], &readfds);
      if (clients[i] > nfds) nfds = clients[i];
    }

    if (select(nfds + 1, &readfds, NULL, NULL, NULL) < 0)
      exit(EXIT_FAILURE);

    if (FD_ISSET(server_fd, &readfds)) {
      int cfd = accept(server_fd, NULL, NULL);
      for (int i = 0; i < FD_SETSIZE; ++i)
        if (clients[i] == -1) { clients[i] = cfd; break; }
    }

    for (int i = 0; i < FD_SETSIZE; ++i) {
      int fd = clients[i];
      if (fd == -1 || !FD_ISSET(fd, &readfds)) continue;
      int n = recv(fd, buf, sizeof(buf), 0);
      if (n <= 0) { close(fd); clients[i] = -1; }
      else send(fd, buf, n, 0);
    }
  }

  return 0;
}
