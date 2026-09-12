#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char **argv) {
  if (argc != 2) {
    dprintf(2, "Usage: ./select_practice <port>\n");
    return 1;
  }

  unsigned int port = atoi(argv[1]);
  (void)port;
  return 0;
}
