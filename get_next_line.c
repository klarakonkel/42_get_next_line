/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkonkel <kkonkel@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 14:42:11 by kkonkel           #+#    #+#             */
/*   Updated: 2024/01/31 16:28:31 by kkonkel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*init_line(char *buffer, int *nl_index);
char	*obtain_line(char *line, char *buffer, int *nl_index, int fd);
size_t	find_nl(char *buffer);

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE + 1];
	char		*line;
	int			nl_index;

	nl_index = -1;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	line = init_line(buffer, &nl_index);
	if (!line)
		return (NULL);
	ft_strlcpy_gnl(buffer, &buffer[nl_index + 1], BUFFER_SIZE + 1);
	line = obtain_line(line, buffer, &nl_index, fd);
	if (!line || line[0] == '\0')
	{
		free(line);
		return (NULL);
	}
	return (line);
}

char	*init_line(char *buffer, int *nl_index)
{
	size_t	i;
	char	*line;

	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	i++;
	line = malloc(sizeof(char) * (i + 1));
	if (!line)
		return (NULL);
	ft_memcpy(line, buffer, i);
	line[i] = '\0';
	if (i > 0 && line[i - 1] == '\n')
		*nl_index = i - 1;
	return (line);
}

size_t	find_nl(char *line)
{
	size_t	i;

	i = -1;
	if (!line)
		return (i);
	while (++i < BUFFER_SIZE)
	{
		if (line[i] == '\n')
			return (i + 1);
		else if (line[i] == '\0')
			return (i);
	}
	return (i);
}

char	*obtain_line(char *line, char *buffer, int *nl_index, int fd)
{
	char	content[BUFFER_SIZE + 1];
	ssize_t	bytes_read;
	size_t	line_len;

	while (*nl_index == -1)
	{
		ft_bzero(content, (BUFFER_SIZE + 1));
		bytes_read = read(fd, content, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(line);
			ft_bzero(buffer, (BUFFER_SIZE + 1));
			return (NULL);
		}
		line_len = find_nl(content);
		ft_strlcpy_gnl(buffer, &content[line_len], (BUFFER_SIZE + 1));
		content[line_len] = '\0';
		line = ft_strjoin_gnl(line, content, nl_index);
		if (bytes_read == 0)
		{
			ft_bzero(buffer, BUFFER_SIZE + 1);
			break ;
		}
	}
	return (line);
}
/*int	main()
{
    int fd = open("text.txt", O_RDONLY);

    if (fd < 0)
        return -1;

    char *line;

    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
	
    close(fd);

    return 0;
}*/