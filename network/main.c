#include <stdio.h>
#include <unistd.h>
#include "server.h"
#include "types.h"

extern char *optarg;
extern int optind;

void display_options();

int main(int argc, char *argv[]) {
  int option;
  char format[] = "dh";
  int mode = RUN_DEFAULT;

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
    default:			// All the other options are considered invalid.
      perror("Invalid command-line options. Please run using -h to list the available options.\n");
      return 1;
    }
  }

  //We need to be able to test the database-server connection separately.
  if (RUN_DEFAULT == mode) {
    run();
  }
  else if(RUN_DB_DEBUG == mode){
    run_debug();
  }
  return 0;
}

void display_options(char *usage) {
  printf("Usage:\n%s [-dh]\n", usage);
  printf("   -d : database debug mode\n");
  printf("   -h : display this help\n");
}