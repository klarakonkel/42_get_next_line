/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkonkel <kkonkel@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 14:42:11 by kkonkel           #+#    #+#             */
/*   Updated: 2024/02/03 13:26:36 by kkonkel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*init_line(char *buffer, int *nl_index);
char	*obtain_line(char *line, char *buffer, int *nl_index, int fd);
size_t	find_nl(char *buffer);

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE + 1]; //buffer for chars read from the file
	char		*line; //line that will be printed
	int			nl_index; //index of the char after '\n', so the first char of the next line

	nl_index = -1;
	if (fd < 0 || BUFFER_SIZE <= 0) //returning null in case of improper fd or buffer size
		return (NULL);
	line = init_line(buffer, &nl_index); //initialising a line to be printed
	if (!line)
		return (NULL);
	ft_strlcpy_gnl(buffer, &buffer[nl_index + 1], BUFFER_SIZE + 1); //leaving just the remaining part of the buffer
	line = obtain_line(line, buffer, &nl_index, fd); //saving the line
	if (!line || line[0] == '\0') //if the line is empty, we free it and return null
	{
		free(line);
		return (NULL);
	}
	return (line); //returning the line that is to be printed
}

char	*init_line(char *buffer, int *nl_index)
{
	size_t	i; //length of a line that is to be printed
	char	*line;

	i = 0;
	while (buffer[i] && buffer[i] != '\n') //skipping chars before \n
		i++;
	i++; //since we broke out of the loop, we are on \n char - we're moving to the next char, the start of a new line
	line = malloc(sizeof(char) * (i + 1)); //we allocate enough memory for the length of the line + the terminating null
	if (!line)
		return (NULL);
	ft_memcpy(line, buffer, i); //we copy the content from buffer to line until i (including \n)
	line[i] = '\0'; //we terminate the line with null char
	if (i > 0 && line[i - 1] == '\n') //if the buffer wasn't empty and didn't start with a \n
		*nl_index = i - 1; //we set the \n char to the appropriate place
	return (line);
}

size_t	find_nl(char *line)
{
	size_t	i;

	i = -1;
	if (!line)
		return (i); //returning -1 if line is empty
	while (++i < BUFFER_SIZE) //increasing before to start from 0
	{
		if (line[i] == '\n') //if we encounter a \n
			return (i + 1); //we return the index of the char after it, so the first char of new line
		else if (line[i] == '\0') //if we reach the end
			return (i); //we return the index of terminating 0
	}
	return (i);
}

char	*obtain_line(char *line, char *buffer, int *nl_index, int fd)
{
	char	content[BUFFER_SIZE + 1];
	ssize_t	bytes_read;
	size_t	line_len;

	while (*nl_index == -1) //in a loop, until we haven't encountered the \n char
	{
		ft_bzero(content, (BUFFER_SIZE + 1)); //we first fill the content buffer with 0s
		bytes_read = read(fd, content, BUFFER_SIZE); //we read from the fd provided into content
		if (bytes_read == -1) //if the reading was not successfull
		{
			free(line); //we free the line
			ft_bzero(buffer, (BUFFER_SIZE + 1)); //and fill buffer with 0s
			return (NULL);
		}
		line_len = find_nl(content); //we search for the length of the line that is to be printed
		ft_strlcpy_gnl(buffer, &content[line_len], (BUFFER_SIZE + 1)); //and copy the REMAINING characters from content to buffer
		content[line_len] = '\0'; //null-terminating
		line = ft_strjoin_gnl(line, content, nl_index); //if we have a part of line left after previously having read from the file,
		if (bytes_read == 0)							//we join this remaining part with the the new part of line (or an empty string if it's not the case)
		{ //if we have read zero bytes from the fd, it means that we have reached the end of the file, and there's nothing more to read
			ft_bzero(buffer, BUFFER_SIZE + 1); //so we fill the buffer with 0s
			break ; //and break out of the loop
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