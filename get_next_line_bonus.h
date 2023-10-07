#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <stdlib.h>
# include <unistd.h>
# include <limits.h>

# define YES -1
# define NO 1

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

char		*get_next_line(int fd);
size_t	ft_strlen(char *s);
char		*ft_strchr_null(char *s, char c);
char		*ft_strjoin_free(char *buffer, char *tmp, int *is_error);
char		*ft_calloc_forone(size_t count, size_t size);
char		*ft_error_found(char *buffer, int *is_error);

#endif
