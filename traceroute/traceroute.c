//Piotr Kałmuczak 292235
#include "traceroute.h"

//Traceroute procedure
int traceroute(char* address){
  //Creating raw socket
  sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
  if(sockfd < 0){
    fprintf(stderr, "socket error: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  //Initializing globals
  pid = getpid();
  targetAddress = address;
  madeTarget = 0;


  //Sending packets
  printf("Traceroute to %s:\n", targetAddress);
  for (int ttl = 1; ttl <= MAX_TTL; ttl++){
    //printf("sending packets for ttl = %d\n", ttl);
    for (int i = 0; i < PACKETS; i++){
      sendPacket(ttl * PACKETS + i, ttl);
    }
    //countAns[ttl] = 0;
    FD_ZERO (&descriptors);
    FD_SET (sockfd, &descriptors);
    tv.tv_sec = MAX_WAIT; tv.tv_usec = 0;
    //printf("starting receiving for ttl = %d\n", ttl);
    receivePacket(ttl);
    printAnswer(ttl);
    if (madeTarget) break;
  }
  return EXIT_SUCCESS;
}

//Calculating checksum
u_int16_t compute_icmp_checksum (const void *buff, int length){

	u_int32_t sum;
	const u_int16_t* ptr = buff;
	assert (length % 2 == 0);
	for (sum = 0; length > 0; length -= 2)
		sum += *ptr++;

	sum = (sum >> 16) + (sum & 0xffff);
	return (u_int16_t)(~(sum + (sum >> 16)));
}

//Printing answers
int printAnswer(int ttl){
  printf("%d. ", ttl);
  if (countAns[ttl] == 0){
    printf("*\n");
    return 0;
  }
  if(strcmp(answers[ttl][0].address, "") != 0)
    printf("%s ", answers[ttl][0].address);
  if (strcmp(answers[ttl][1].address, answers[ttl][0].address) != 0)
    printf("%s ", answers[ttl][1].address);
  if ((strcmp(answers[ttl][2].address, answers[ttl][0].address) != 0) &&
      (strcmp(answers[ttl][2].address, answers[ttl][1].address) != 0))
    printf("%s ", answers[ttl][1].address);

  if (countAns[ttl] != 3){
    printf("???\n");
    return 0;
  }

  int avgTime =  answers[ttl][0].send.tv_sec * 1000 + answers[ttl][0].send.tv_usec / 1000;
         avgTime += answers[ttl][1].send.tv_sec * 1000 + answers[ttl][1].send.tv_usec / 1000;
         avgTime += answers[ttl][2].send.tv_sec * 1000 + answers[ttl][2].send.tv_usec / 1000;
  avgTime /= 3;
  printf("%dms\n", avgTime);
  return 0;
}
