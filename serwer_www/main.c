//Piotr Kałmuczak 292235
#include "stdio.h"
#include "stdlib.h"
#include "sys/stat.h"
#include "errno.h"
#include <string.h>
#include "server.h"

int main(int argc, char* argv[]){

  //Checking input
  if (argc != 3){
    printf("Invalid input - try: sudo ./server_www <port> <catalog>\n" );
    return 0;
  }

  int port = atoi(argv[1]);
  char* catalog = argv[2];
  struct stat ctlg = {0};

  if (port < 1 || port > 65535){
    printf("Invalid port - possible range is [1, 65535]\n");
    return 0;
  }

  if (stat(catalog, &ctlg))
    if ( !(S_ISDIR(ctlg.st_mode)) ){
      printf("Invalid catalog - %s doesn't exist\n", catalog);
      return 0;
    }

  if(server(port, catalog) < 0){
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
