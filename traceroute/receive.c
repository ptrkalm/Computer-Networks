//Piotr Kałmuczak 292235
#include "traceroute.h"

//Receiving packet
int receivePacket(int ttl){
  struct timeval beggining, now;
  long int elapsed;

  gettimeofday(&beggining, NULL);

  for (;;) {
		struct sockaddr_in 	sender;
		socklen_t sender_len = sizeof(sender);
		u_int8_t buffer[IP_MAXPACKET];

    int ready = select(sockfd+1, &descriptors, NULL, NULL, &tv);
    if (ready < 0){
      fprintf(stderr, "select error: %s\n", strerror(errno));
      return EXIT_FAILURE;
    }
    if (ready == 0){
      return EXIT_SUCCESS;
    }

    ssize_t packet_len = recvfrom (sockfd, buffer, IP_MAXPACKET, MSG_DONTWAIT, (struct sockaddr*)&sender, &sender_len);
		if (packet_len < 0) {
			fprintf(stderr, "recvfrom error: %s\n", strerror(errno));
			return EXIT_FAILURE;
		}

		struct iphdr* ip_header = (struct iphdr*) buffer;
		u_int8_t* icmp_packet = buffer +  4 * ip_header->ihl;
    struct icmphdr* icmp_header = (struct icmphdr*) icmp_packet;

    if(icmp_header->type == ICMP_TIME_EXCEEDED){
			icmp_packet = ((u_int8_t*) icmp_header) + 8;
			struct iphdr* temp_ip_header = (struct iphdr*) icmp_packet;
			icmp_packet = ((u_int8_t*) temp_ip_header) + 4 * temp_ip_header->ihl;
      icmp_header = (struct icmphdr*) icmp_packet;
    }

    if (icmp_header->un.echo.id != pid ||
        icmp_header->un.echo.sequence >= (ttl + 1) * PACKETS ||
        icmp_header->un.echo.sequence < ttl * PACKETS){
      continue;
	  }

    int id = icmp_header->un.echo.sequence - PACKETS*ttl;
    answers[ttl][id].send.tv_usec = 1000000*MAX_WAIT - tv.tv_usec;
    countAns[ttl]++;

    inet_ntop(AF_INET, &(sender.sin_addr), answers[ttl][id].address, 20);
    if (strcmp(answers[ttl][id].address, targetAddress) == 0){
      madeTarget = 1;
    }

    gettimeofday(&now, NULL);
    elapsed = now.tv_sec * 1000000 + now.tv_usec - (beggining.tv_sec * 1000000 + beggining.tv_usec);
    if (elapsed > 1000000 * MAX_WAIT)
      break;
  }

	return EXIT_SUCCESS;
}
