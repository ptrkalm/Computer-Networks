//Piotr Kałmuczak 292235
#include "transport.h"
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int receivePacket(){
  tv.tv_sec = 0; tv.tv_usec = 100000;
  FD_ZERO(&descriptors);
  FD_SET(sockfd, &descriptors);

  for (;;){
    struct sockaddr_in  sender;
		socklen_t           sender_len = sizeof(sender);

    int ready = select(sockfd+1, &descriptors, NULL, NULL, &tv);
    if (ready < 0){
      fprintf(stderr, "select error: %s\n", strerror(errno));
      return EXIT_FAILURE;
    }
    if (ready == 0){
      return EXIT_SUCCESS;
    }

    ssize_t datagram_len = recvfrom (sockfd, buffer, IP_MAXPACKET, 0,
      (struct sockaddr*)&sender, &sender_len);
		if (datagram_len < 0) {
			fprintf(stderr, "recvfrom error: %s\n", strerror(errno));
			return EXIT_FAILURE;
		}

    char sender_ip_str[20];
		inet_ntop(AF_INET, &(sender.sin_addr), sender_ip_str, sizeof(sender_ip_str));
    //Tutaj zaczynam zapisywanie danych jezeli zgadza się IP
    if (strcmp(sender_ip_str, "156.17.4.30") == 0){
      int retStart, retLength;
      sscanf(buffer, "%*s %d %d", &retStart, &retLength);
      if (received[retStart/1000]){
        continue; //Jezeli to duplikat to olewam
      }
      received[retStart/1000] = 1; //Zapamietuje, ze ta DATA juz przyszla
      int buffIdx = retStart/length;
      memcpy(tempBuffers[buffIdx], buffer, 1020);
      tempBuffersLen[buffIdx] = datagram_len;
    }
  }

  return EXIT_SUCCESS;
}
