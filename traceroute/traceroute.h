//Piotr Kałmuczak 292235

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <netinet/ip_icmp.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <netinet/ip.h>
#include <stdint.h>

#define MAX_TTL 30
#define PACKETS 3
#define MAX_WAIT 1

char* targetAddress;
int sockfd;
int madeTarget;
pid_t pid;

fd_set descriptors;
struct timeval tv;

struct answerInfo{
  struct timeval send;
  char address[20];
};

struct answerInfo answers[MAX_TTL+1][PACKETS];
int countAns[MAX_TTL+1];

int traceroute(char* address);
u_int16_t compute_icmp_checksum (const void *buff, int length);
int printAnswer(int ttl);
int receivePacket(int ttl);
int sendPacket(int id, int ttl);
