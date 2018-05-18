//Piotr Kałmuczak 292235
#include "traceroute.h"

int main(int argc, char *argv[]){

  //Checking input
  if (argc != 2){
    printf("Invalid input - try: sudo ./traceroute <IP address>\n");
    return 0;
  }

  struct sockaddr_in sender;
  char sender_ip_str[20];
  if(inet_ntop(AF_INET, &(sender.sin_addr), sender_ip_str, sizeof(sender_ip_str)) == 0){
    printf("Invalid IP address\n");
    return EXIT_FAILURE;
  }

  //Initializing traceroute
  if(traceroute(argv[1]) < 0){
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
