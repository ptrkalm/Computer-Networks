//Piotr Kałmuczak 292235
#include <sys/select.h>
#include <arpa/inet.h>
#include <stdio.h>
#define IN_TIME 1

fd_set descriptors;
struct timeval tv;

int sockfd;
int start;
int length;

int port;
char* output_file;
int size;

struct sockaddr_in server_address;
char buffer[1020];
char tempBuffers[100001][1020];
int  tempBuffersLen[100001];
int received[100001];
FILE * file;

int transport();
int sendPacket(int start, int length);
int receivePacket();
