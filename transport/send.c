//Piotr Kałmuczak 292235
#include "transport.h"
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

//Sending packet
int sendPacket(int start, int length){
  char message[20];
  int message_len = snprintf(message, 20, "GET %d %d\n", start, length);
  if (sendto(sockfd, message, message_len, 0,
      (struct sockaddr*) &server_address,
      sizeof(server_address)) != message_len) {
		fprintf(stderr, "sendto error: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

  return EXIT_SUCCESS;
}
