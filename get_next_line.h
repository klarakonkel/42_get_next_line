/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkonkel <kkonkel@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 18:36:36 by kkonkel           #+#    #+#             */
/*   Updated: 2024/01/31 11:30:06 by kkonkel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 5
# endif
# if BUFFER_SIZE > 99999 || BUFFER_SIZE < 1
#  undef BUFFER_SIZE
#  define BUFFER_SIZE 5
# endif

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>

char	*get_next_line(int fd);
char	*init_line(char *buffer, int *nl_index);
char	*obtain_line(char *line, char *buffer, int *nl_index, int fd);
size_t	find_nl(char *line);
size_t	ft_strlen(const char *s);
void	ft_strlcpy_gnl(char *dst, const char *src, size_t dstsize);
void	*ft_memcpy(void *dst, const void *src, size_t n);
char	*ft_strjoin_gnl(char *s1, char *s2, int *nl_index);
void	ft_bzero(void *s, size_t n);

#endif