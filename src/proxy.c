#define _GNU_SOURCE
#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
  int gateway;

  if ((gateway = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    return 1;
  }

  struct sockaddr_in gateway_addr;
  gateway_addr.sin_family = AF_INET;
  gateway_addr.sin_addr.s_addr = INADDR_ANY;
  gateway_addr.sin_port = htons(25565);

  if (bind(gateway, (struct sockaddr *)&gateway_addr, sizeof(gateway_addr)) <
      0) {
    return 1;
  }

  if (setsockopt(gateway, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) <
      0) {
    return 1;
  }

  if (listen(gateway, SOMAXCONN)) {
    return 1;
  }

  int downstream;

  if ((downstream = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    return 1;
  }

  struct sockaddr_in downstream_addr;
  downstream_addr.sin_family = AF_INET;
  downstream_addr.sin_addr.s_addr = inet_addr("127.0.0.1"),
  downstream_addr.sin_port = htons(35565);

  if (connect(downstream, (struct sockaddr *)&downstream_addr,
              sizeof(downstream_addr)) < 0) {
    return 1;
  }

  int client = accept4(gateway, NULL, NULL, 0);
  struct pollfd fds[2] = {
      {.fd = client, .events = POLLIN},
      {.fd = downstream, .events = POLLIN},
  };

  while (poll(fds, 2, -1) > 0) {
    char buf[4096];

    if (fds[0].revents & POLLIN) {
      ssize_t n = recv(client, buf, sizeof buf, 0);
      if (n <= 0)
        break;
      send(downstream, buf, n, 0);
    }
    if (fds[1].revents & POLLIN) {
      ssize_t n = recv(downstream, buf, sizeof buf, 0);
      if (n <= 0)
        break;
      send(client, buf, n, 0);
    }
  }

  return 0;
}
