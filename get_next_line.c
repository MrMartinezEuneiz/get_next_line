/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikemart <mikemart@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 17:58:45 by mikemart          #+#    #+#             */
/*   Updated: 2025/04/05 17:58:45 by mikemart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*extract_line(char *stash)
{
	size_t	i;

	i = 0;
	if (!stash || !stash[0])
		return (NULL);
	while (stash[i] && stash[i] != '\n')
		i++;
	if (stash[i] == '\n')
		i++;
	return (gnl_substr(stash, 0, i));
}

static char	*clean_stash(char *stash)
{
	size_t	i;
	char	*new_stash;

	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (!stash[i])
	{
		free(stash);
		return (NULL);
	}
	new_stash = gnl_substr(stash, i + 1, gnl_strlen(stash) - i);
	free(stash);
	return (new_stash);
}

static char	*read_and_stash(int fd, char *stash)
{
	char	*buf;
	ssize_t	bytes;

	buf = malloc(BUFFER_SIZE + 1);
	if (!buf)
		return (NULL);
	bytes = 1;
	while (!gnl_strchr(stash, '\n') && bytes > 0)
	{
		bytes = read(fd, buf, BUFFER_SIZE);
		if (bytes < 0)
		{
			free(buf);
			free(stash);
			return (NULL);
		}
		buf[bytes] = '\0';
		stash = gnl_strjoin(stash, buf);
	}
	free(buf);
	return (stash);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	stash = read_and_stash(fd, stash);
	if (!stash)
		return (NULL);
	line = extract_line(stash);
	stash = clean_stash(stash);
	return (line);
}

// cc -Wall -Werror -Wextra -D BUFFER_SIZE=42
//get_next_line.c get_next_line_utils.c main.c -o test

/* #include <stdio.h>
#include <fcntl.h>
#include "get_next_line.h"
int main()
{
	int fd = open("a.txt", O_RDONLY);
	char *a;
	a = get_next_line(fd);
	while (a != NULL)
	{
		printf("%s", a);
		free(a);
		a = get_next_line(fd);
	}

	printf("%s", a);
	free(a);

	return 0;
} 
#include <fcntl.h>
#include "get_next_line.h"
int main()
{
	int fd = open("a.txt", O_RDONLY);
	char *a;
	fd =1
	a = get_next_line(fd);
	while (a != NULL)
	{
		printf("%s", a);
		free(a);
		a = get_next_line(fd);
	}

	printf("%s", a);
	free(a);

	return 0;	
*/