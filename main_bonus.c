#include <fcntl.h>
#include <stdio.h>
#include "get_next_line_bonus.h"

int main()
{
  char *line;
  int fd1 = open("a.txt", O_RDONLY);
  int fd2 = open("b.txt", O_RDONLY);
  int fd3 = open("c.txt", O_RDONLY);
  char *newline;
  printf("fd1: %d\n", fd1);
  printf("fd2: %d\n", fd2);
  printf("fd3: %d\n", fd3);
  int finish_count = 0;
  int idx = 0;
  while (finish_count < 3)
  {
      if (idx % 3 == 0)
      {
          printf("\n---------- fd1: %d ----------\n", fd1);
          
          newline = get_next_line(fd1);
          printf("newline:  \t%s\n", newline);
          free(newline);
          if (newline != NULL)
              finish_count = 0;
          else
              finish_count++;
      }
      else if(idx % 3 == 1)
      {
          printf("\n---------- fd2: %d ----------\n", fd2);
          newline = get_next_line(fd2);
          printf("newline:  \t%s\n", newline);
          free(newline);
          if (newline != NULL)
              finish_count = 0;
          else
              finish_count++;
      }
      else if (idx % 3 == 2)
      {
          printf("\n---------- fd3: %d ----------\n", fd3);
          newline = get_next_line(fd3)  ;
          printf("newline:  \t%s\n", newline);
          free(newline);
          if (newline != NULL)
              finish_count = 0;
          else
              finish_count++;
      }
      idx++;
  }
  close(fd1);
  close(fd2);
  close(fd3);
  return 0;
}

// cc -Wall -Wextra -Werror -D BUFFER_SIZE=10 main_bonus.c get_next_line_bonus.c  get_next_line_utils_bonus.c 