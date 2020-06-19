/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sselusa <sselusa@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/17 10:19:36 by sselusa           #+#    #+#             */
/*   Updated: 2020/06/17 12:45:38 by sselusa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dasm.h"

static unsigned int		reverse4(unsigned int x)
{
    x = (x & 0x00FF00FF) <<  8 | (x & 0xFF00FF00) >>  8;
    x = (x & 0x0000FFFF) << 16 | (x & 0xFFFF0000) >> 16;
    return x;
}

static char				*get_output(char *filename)
{
	char				*output;
	char				*path;
	size_t				len;

	len = (size_t)(ft_strrchr(filename, '.') - filename);
	path = ft_strsub(filename, 0, len);
	if (!path)
		return (NULL);
	output = ft_strjoin(path, ".test");
	free(path);
	return (output);
}

static void				get_magic(int fd)
{
	void				*mem;
	unsigned int		magic;

	mem = ft_memalloc(sizeof(unsigned int));
	read(fd, mem, sizeof(unsigned int));
	magic = reverse4(*(unsigned int*)mem);
	free(mem);
	if (magic == COREWAR_EXEC_MAGIC)
		ft_printf("Magic\tOK\t%#x\n", magic);
	else
		ft_printf("Magic\tKO\t%#x\t(%#x)\n", magic, COREWAR_EXEC_MAGIC);
}

static void				get_name(int fd, int wr)
{
	void				*mem;
	char				*name;

	mem = ft_memalloc(PROG_NAME_LENGTH);
	read(fd, mem, PROG_NAME_LENGTH);
	name = (char*)mem;
	ft_printf("Name\tOK\t%s\n", name);
	ft_putstr_fd(NAME_CMD_STRING, wr);
	ft_putstr_fd("\t\"", wr);
	ft_putstr_fd(name, wr);
	ft_putendl_fd("\"", wr);
	free(mem);
}

static void				skip_null(int fd)
{
	void				*mem;

	mem = ft_memalloc(sizeof(unsigned int));
	read(fd, mem, 4);
	free(mem);
}

static void				get_size(int fd)
{
	void				*mem;
	size_t				size;

	mem = ft_memalloc(sizeof(unsigned int));
	read(fd, mem, sizeof(unsigned int));
	size = reverse4(*(unsigned int*)mem);
	free(mem);
	if (size <= CHAMP_MAX_SIZE)
		ft_printf("Size\tOK\t%zu\n", size);
	else
		ft_printf("Size\tKO\t%zu\t(MAX = %zu)\n", size, CHAMP_MAX_SIZE);
}

static void				get_comment(int fd, int wr)
{
	void				*mem;
	char				*comment;

	mem = ft_memalloc(COMMENT_LENGTH);
	read(fd, mem, COMMENT_LENGTH);
	comment = (char*)mem;
	ft_printf("Comment\tOK\t%s\n", comment);
	ft_putstr_fd(COMMENT_CMD_STRING, wr);
	ft_putstr_fd("\t\"", wr);
	ft_putstr_fd(comment, wr);
	ft_putendl_fd("\"", wr);
	free(comment);
}

int						parse(char *filename)
{
	int					fd_rd;
	int					fd_wr;
	char				*output;
	mode_t				mode;

	mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	fd_rd = open(filename, O_RDONLY);
	if (fd_rd == -1)
		return (0);
	if (!(output = get_output(filename)))
		return (0);
	fd_wr = open(output, O_WRONLY | O_CREAT | O_TRUNC, mode);
	free(output);
	get_magic(fd_rd);
	get_name(fd_rd, fd_wr);
	skip_null(fd_rd);
	get_size(fd_rd);
	get_comment(fd_rd, fd_wr);
	// PARSE
	skip_null(fd_rd);
	close(fd_rd);
	close(fd_wr);
	return (1);
}
