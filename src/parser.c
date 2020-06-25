/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sselusa <sselusa@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/17 10:19:36 by sselusa           #+#    #+#             */
/*   Updated: 2020/06/25 12:40:29 by sselusa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dasm.h"

unsigned int			reverse4(unsigned int x)
{
    x = (x & 0x00FF00FF) <<  8 | (x & 0xFF00FF00) >>  8;
    x = (x & 0x0000FFFF) << 16 | (x & 0xFFFF0000) >> 16;
    return (x);
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

static void				get_header(t_parser *p)
{
	read(p->fd_input, &(p->header.magic), sizeof(unsigned int));
	p->header.magic = reverse4(p->header.magic);
	read(p->fd_input, p->header.prog_name, PROG_NAME_LENGTH);
	p->header.prog_name[PROG_NAME_LENGTH] = '\0';
	lseek(p->fd_input, DIV_SIZE, SEEK_CUR);
	read(p->fd_input, &(p->header.prog_size), sizeof(unsigned int));
	p->header.prog_size = reverse4(p->header.prog_size);
	read(p->fd_input, p->header.comment, COMMENT_LENGTH);
	p->header.comment[COMMENT_LENGTH] = '\0';
	lseek(p->fd_input, DIV_SIZE, SEEK_CUR);
}

static void				write_header(t_parser *p)
{
	ft_putstr_fd(NAME_CMD_STRING, p->fd_output);
	ft_putstr_fd("\t\"", p->fd_output);
	ft_putstr_fd(p->header.prog_name, p->fd_output);
	ft_putendl_fd("\"", p->fd_output);
	ft_putstr_fd(COMMENT_CMD_STRING, p->fd_output);
	ft_putstr_fd("\t\"", p->fd_output);
	ft_putstr_fd(p->header.comment, p->fd_output);
	ft_putendl_fd("\"", p->fd_output);
	ft_putchar_fd('\n', p->fd_output);
}

static void				parser_debug(t_parser *p)
{
	ft_printf("Input file descriptor: %d\n", p->fd_input);
	ft_printf("Input filename: %s\n", p->input_filename);
	ft_printf("Output file descriptor: %d\n", p->fd_output);
	ft_printf("Output filename: %s\n", p->output_filename);
	ft_printf("Open mode: %#x\n", p->openmode);
	ft_printf("Header:\n\tMagic number: %#x\n", p->header.magic);
	ft_printf("\tProgram name: %s\n", p->header.prog_name);
	ft_printf("\tProgram size: %u\n", p->header.prog_size);
	ft_printf("\tComment: %s\n", p->header.comment);
}

int						parse(char *filename)
{
	t_parser			p;

	ft_bzero(&p, sizeof(t_parser));
	p.input_filename = filename;
	p.openmode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	p.fd_input = open(p.input_filename, O_RDONLY);
	if (p.fd_input == -1)
		return (0);
	if (!(p.output_filename = get_output(p.input_filename)))
		return (0);
	p.fd_output = open(p.output_filename,
		O_WRONLY | O_CREAT | O_TRUNC, p.openmode);
	get_header(&p);
	parser_debug(&p);
	write_header(&p);
	prog(&p);
	close(p.fd_input);
	close(p.fd_output);
	free(p.output_filename);
	return (1);
}
