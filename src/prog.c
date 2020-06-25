/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prog.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sselusa <sselusa@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/24 22:51:40 by sselusa           #+#    #+#             */
/*   Updated: 2020/06/25 13:14:04 by sselusa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dasm.h"

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
			{
			read(p->fd_input, &(c->arg[argno]), c->tdirsize);
			c->size += c->tdirsize;
			if (c->tdirsize == 4)
				c->arg[argno] = reverse4(c->arg[argno]);
			}
		}
		else if (c->argtype[argno] == T_IND)
		{
			read(p->fd_input,  &(c->arg[argno]), 2);
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

	ft_printf("\t%s ", c->name);
	ft_putstr_fd(c->name, p->fd_output);
	ft_putchar_fd(' ', p->fd_output);
	argno = 0;
	while (argno < c->argc)
	{
		if (c->mark[argno])
		{
			ft_printf("%c", c->mark[argno]);
			ft_putchar_fd(c->mark[argno], p->fd_output);
		}
		ft_putnbr_fd(c->arg[argno], p->fd_output);
		ft_printf("%d", c->arg[argno]);
		if (argno == (c->argc - 1))
		{
			ft_putchar_fd('\n', p->fd_output);
			ft_printf("\n");
		}
		else
		{
			ft_putchar_fd(SEPARATOR_CHAR, p->fd_output);
			ft_printf("%c", SEPARATOR_CHAR);
		}
		argno++;
	}
	
}

void					prog(t_parser *p)
{
	t_command			c;
	unsigned int		pos;

	pos = 0;
	ft_printf("\nExec code:\n");
	while (pos < p->header.prog_size)
	{
		get_command(p, &c);
		write_command(p, &c);
		pos += c.size;
	}
}
