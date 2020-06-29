/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sselusa <sselusa@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/17 10:19:36 by sselusa           #+#    #+#             */
/*   Updated: 2020/06/29 22:42:19 by sselusa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dasm.h"

unsigned int			reverse4(unsigned int x)
{
    x = (x & 0x00FF00FF) <<  8 | (x & 0xFF00FF00) >>  8;
    x = (x & 0x0000FFFF) << 16 | (x & 0xFFFF0000) >> 16;
    return (x);
}

unsigned int			reverse2(unsigned int x)
{
    x = (x & 0x00FF00FF) <<  8 | (x & 0xFF00FF00) >>  8;
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

static t_arg_type		check_argtype(unsigned int argno, unsigned char typecode)
{
	unsigned char		code;

	code = ((3 << (8 - (2 * argno))) & typecode) >> (8 - (2 * argno));
	if (code == REG_CODE)
		return (T_REG);
	else if (code == DIR_CODE)
		return (T_DIR);
	else if (code == IND_CODE)
		return (T_IND);
	else
		return (0);
}

static void				set_argtypes(t_command *c)
{
	if (c->typecode)
	{
		c->argtype[0] = check_argtype(1, c->typecode);
		c->argtype[1] = check_argtype(2, c->typecode);
		c->argtype[2] = check_argtype(3, c->typecode);
	}
	else
	{
		c->argtype[0] = op_tab[c->code - 1].argtype[0];
		c->argtype[1] = op_tab[c->code - 1].argtype[1];
		c->argtype[2] = op_tab[c->code - 1].argtype[2];
	}
}

static void				set_marks(t_command *c)
{
	unsigned int		argno;

	argno = 0;
	while (argno < c->argc)
	{
		if (c->argtype[argno] == T_REG)
			c->mark[argno] = REGISTRY_CHAR;
		else if (c->argtype[argno] == T_DIR)
			c->mark[argno] = DIRECT_CHAR;
		argno++;
	}
}

static void				get_arguments(t_parser *p, t_command *c)
{
	unsigned char		argno;

	argno = 0;
	while (argno < c->argc)
	{
		if (c->argtype[argno] == T_REG)
		{
			read(p->fd_input, &(c->arg[argno]), 1);
			c->size += 1;
		}
		else if (c->argtype[argno] == T_DIR)
		{
			read(p->fd_input, &(c->arg[argno]), c->tdirsize);
			c->size += c->tdirsize;
			if (c->tdirsize == 4)
				c->arg[argno] = (int)reverse4(c->arg[argno]);
			else
				c->arg[argno] = (short)reverse2(c->arg[argno]);
		}
		else if (c->argtype[argno] == T_IND)
		{
			read(p->fd_input,  &(c->arg[argno]), 2);
			c->arg[argno] = reverse2(c->arg[argno]);
			c->size += 2;
		}
		argno++;
	}
}

static void				get_command(t_parser *p, t_command *c)
{
	ft_bzero(c, sizeof(t_command));
	read(p->fd_input, &(c->code), 1);
	c->size += 1;
	c->name = op_tab[c->code - 1].name;
	c->typecode = 0;
	if (op_tab[c->code - 1].typecode)
	{
		read(p->fd_input, &(c->typecode), 1);
		c->size += 1;
	}
	c->tdirsize = op_tab[c->code - 1].tdirsize ? 2 : 4;
	c->argc = op_tab[c->code - 1].argc;
	set_argtypes(c);
	set_marks(c);
	get_arguments(p, c);
}

static void				write_command(t_parser *p, t_command *c)
{
	unsigned int		argno;

	ft_putstr_fd(c->name, p->fd_output);
	ft_putchar_fd(' ', p->fd_output);
	argno = 0;
	while (argno < c->argc)
	{
		if (c->mark[argno])
			ft_putchar_fd(c->mark[argno], p->fd_output);
		ft_putnbr_fd(c->arg[argno], p->fd_output);
		if (argno == (c->argc - 1))
			ft_putchar_fd('\n', p->fd_output);
		else
			ft_putchar_fd(SEPARATOR_CHAR, p->fd_output);
		argno++;
	}
	
}

static void				prog(t_parser *p)
{
	t_command			c;
	unsigned int		pos;

	pos = 0;
	while (pos < p->header.prog_size)
	{
		get_command(p, &c);
		write_command(p, &c);
		pos += c.size;
	}
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
	write_header(&p);
	prog(&p);
	close(p.fd_input);
	close(p.fd_output);
	free(p.output_filename);
	return (1);
}
