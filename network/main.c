#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "server.h"
#include "types.h"

extern char *optarg;
extern int optind;

void display_options();

int main(int argc, char *argv[]) {
  int option;
  char format[] = "dhi:p:";
  int mode = RUN_DEFAULT;
  char *ip = IP_DEFAULT;
  int port = PORT_DEFAULT;
  char *log_file = LOG_PATH_DEFAULT;
  char *db_url = LOCAL_DB_INFO;

  while ((option = getopt(argc, argv, format)) != -1) {
    switch (option) {
    case 'd':
      mode = RUN_DB_DEBUG;
      printf("Launching database debug mode...\n");
      break;
    case 'h':
      mode = RUN_EXIT;
      display_options(argv[0]);
      break;
    case 'i':
      ip = optarg;
      printf("%s\n", ip);
      break;
    case 'p':
      //No checks are performed on the port, if a wrong port or format is given
      //Program will fail.
      port = (int) strtol(optarg, NULL, 10);
      printf("Interpreted port as %d\n", port);
      break;
    default:			// All the other options are considered invalid.
      perror("Invalid command-line options. Please run using -h to list the available options.\n");
      return 1;
    }
  }

  //We need to be able to test the database-server connection separately.
  if (RUN_DEFAULT == mode) {
    run(ip, port, log_file, db_url);
  }
  else if(RUN_DB_DEBUG == mode){
    run_debug(db_url);
  }
  return 0;
}

void display_options(char *usage) {
  printf("Usage:\n%s [-dh] [-i ip] [-p port]\n", usage);
  printf("   -d : database debug mode\n");
  printf("   -h : display this help\n");
}