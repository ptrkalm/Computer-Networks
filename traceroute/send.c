//Piotr Kałmuczak 292235
#include "traceroute.h"

//Sending packet
int sendPacket(int id, int ttl){
  struct icmphdr icmp_header;
  icmp_header.type = ICMP_ECHO;
  icmp_header.code = 0;
  icmp_header.un.echo.id = pid;
  icmp_header.un.echo.sequence = id;
  icmp_header.checksum = 0;
  icmp_header.checksum =
    compute_icmp_checksum ((u_int16_t*)&icmp_header,
                          sizeof(icmp_header));

  struct sockaddr_in recipient;
  bzero (&recipient, sizeof(recipient));
  recipient.sin_family = AF_INET;

  inet_pton(AF_INET, targetAddress, &recipient.sin_addr);

  setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(int));

  if (sendto(sockfd, &icmp_header, sizeof(icmp_header), 0,
      (struct sockaddr*)&recipient, sizeof(recipient)) < 0){
        fprintf(stderr, "sendPacket error: %s\n", strerror(errno));
        return EXIT_FAILURE;
      }

  return 0;
}
