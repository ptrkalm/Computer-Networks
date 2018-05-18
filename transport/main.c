//Piotr Kałmuczak 292235
#include "transport.h"
#include "stdio.h"
#include "stdlib.h"

int main(int argc, char*argv[]){

  //Checking input
  if (argc != 4){
    printf("Invalid input - try: sudo ./transport <port> <output_file> <size_in_bytes>\n");
    return 0;
  }

  port = atoi(argv[1]);
  output_file = argv[2];
  size = atoi(argv[3]);
  if (port < 1 || port > 65535){
    printf("Invalid port - possible range is [1, 65535]\n");
    return 0;
  }
  if (size < 1){
    printf("Invalid size - possible range is [1, 1000100]\n");
    return 0;
  }

  //Initializing transport
  if(transport() < 0){
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
