#include <event2/event.h>
#include <event2/util.h>
#include <event2/dns.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void timer_callback(evutil_socket_t fd, short what, void *arg) {
  printf("Timer fired\n");
}

void resolve_callback(int result, struct evutil_addrinfo *res, void *arg) {
  printf("Resolve callback\n");
  struct evdns_base *base = (struct evdns_base *) arg;
  evdns_base_free(base, 0);
}

void main() {
  struct event_base *base;
  base = event_base_new();

  struct evutil_addrinfo hints;
  struct evutil_addrinfo *answer = NULL;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  struct evdns_base* dns_base = evdns_base_new(base, 1);

  if(dns_base == NULL) {
    printf("Error initializing DNS base\n");
    exit(1);
  }

  struct evdns_getaddrinfo_request *req = evdns_getaddrinfo(dns_base, "www.google.com", "https", &hints, resolve_callback, dns_base);

  int err = event_base_loop(base, EVLOOP_ONCE);
}
