
#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>

#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

unsigned short checksum(unsigned short *buf, int bufsz)
{
  unsigned long sum = 0;

  while (bufsz > 1) {
    sum += *buf;
    buf++;
    bufsz -= 2;
  }

  if (bufsz == 1) {
    sum += *(unsigned char *)buf;
  }

  sum = (sum & 0xffff) + (sum >> 16);
  sum = (sum & 0xffff) + (sum >> 16);

  return ~sum;
}

int
main(int argc, char *argv[])
{
  int sock;
  struct icmphdr hdr;
  struct sockaddr_in addr;
  int n;

  char buf[2000];
  struct icmphdr *icmphdrptr;
  struct iphdr *iphdrptr;

  if (argc != 2) {
    printf("usage : %s IPADDR\n", argv[0]);
    return 1;
  }

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(argv[1]);

  sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
  if (sock < 0) {
    perror("socket");
    return 1;
  }

  memset(&hdr, 0, sizeof(hdr));

  hdr.type = ICMP_ECHO;
  hdr.code = 0;
  hdr.checksum = 0;
  hdr.un.echo.id = 0;
  hdr.un.echo.sequence = 0;

  hdr.checksum = checksum((unsigned short *)&hdr, sizeof(hdr));

  n = sendto(sock,
             (char *)&hdr, sizeof(hdr),
             0, (struct sockaddr *)&addr, sizeof(addr));
  if (n < 1) {
    perror("sendto");
  }

  memset(buf, 0, sizeof(buf));

  n = recv(sock, buf, sizeof(buf), 0);
  if (n < 1) {
    perror("recv");
  }

  iphdrptr = (struct iphdr *)buf;

  icmphdrptr = (struct icmphdr *)(buf + (iphdrptr->ihl * 4));

  if (icmphdrptr->type == ICMP_ECHOREPLY) {
    printf("received ICMP ECHO REPLY\n");
  } else {
    printf("received ICMP %d\n", icmphdrptr->type);
  }

  close(sock);

  return 0;
}
