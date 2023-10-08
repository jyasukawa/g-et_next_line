#include <stdio.h>
#include <fcntl.h>
#include "get_next_line.h"

int main(int argc, char *argv[])
{
  char *file_name;

  if (argc == 1)
      file_name = "test.txt";
  else
      file_name = argv[1];

  int fd = open(file_name, O_RDONLY);
  char *nextline;

  printf("\n[START] %s\n\tfd: %d\n\n", file_name, fd);
  do{
      nextline = get_next_line(fd);
      printf("nextline:  \t%s\n", nextline);
      free(nextline);
  }while (nextline != NULL);

  close(fd);
}

// cc -Wall -Wextra -Werror -D BUFFER_SIZE=10 main.c get_next_line.c get_next_line_utils.c