//Piotr Kałmuczak 292235
#include "transport.h"
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>

#define IN_TIME 1

void writeToFile(char* buff, int dataLength, int ts, int tl){
  int gap = 0;
  if (ts == 0) gap = 1;
  else while (ts > 0) {ts /= 10; gap++;}
  while (tl > 0) {tl /= 10; gap++;}

  for (int i = 7 + gap; i < dataLength; i++){
    fputc(buff[i], file);
  }
}

int transport(){
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		fprintf(stderr, "socket error: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

  bzero (&server_address, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_port   = htons(port);
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  if (bind (sockfd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0){
    fprintf(stderr, "bind error: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }
  inet_pton(AF_INET, "156.17.4.30", &server_address.sin_addr);
  start = 0;
  length = 1000;
  int retStart, retLength;

  file = fopen(output_file, "w");
  if (!file){
    fprintf(stderr, "fopen error: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  while(start < size){
    if (size < length){
      sendPacket(start, size);
      receivePacket();
      if(strcmp(tempBuffers[0], "") != 0){
        sscanf(tempBuffers[0], "%*s %d %d", &retStart, &retLength);
        writeToFile(tempBuffers[0], tempBuffersLen[0], retStart, retLength);
        break;
      }
      continue;
    }
    if (start + length > size){
      int tempLen = size - start;
      sendPacket(start, tempLen);
    }
    else{
      sendPacket(start, length);
      for (int i = 1; i < 1000; i++){
        if (start + length*i + length > size){
          int tempLen = size - start;
          sendPacket(start + length*i, tempLen);
          break;
        }
        if (!(received[(start + length*i)/1000]) && start + length*i < size)
          sendPacket(start + length*i, length);
      }
    }
    receivePacket();
    for (int i = start/1000; i < 100001; i++){
      if(strcmp(tempBuffers[i], "") == 0) break;//Zapisuje tylko prefix
      sscanf(tempBuffers[i], "%*s %d %d", &retStart, &retLength);
      writeToFile(tempBuffers[i], tempBuffersLen[i], retStart, retLength);
      //Przesuwam okno i czyszczę tempBuffers[i]
      start += length;
      strcpy(tempBuffers[i], "");
    }
  }

  fclose(file);

  printf("transport done\n");
  close (sockfd);
  return EXIT_SUCCESS;
}
