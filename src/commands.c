/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sselusa <sselusa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/30 00:02:27 by sselusa           #+#    #+#             */
/*   Updated: 2020/07/05 14:21:09 by sselusa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dasm.h"

static void				get_argtypes(t_command *c)
{
	if (c->typecode)
	{
		c->argtype[0] = check_argtype(1, c->typecode);
		c->argtype[1] = check_argtype(2, c->typecode);
		c->argtype[2] = check_argtype(3, c->typecode);
	}
	else
	{
		c->argtype[0] = g_op_tab[c->code - 1].argtype[0];
		c->argtype[1] = g_op_tab[c->code - 1].argtype[1];
		c->argtype[2] = g_op_tab[c->code - 1].argtype[2];
	}
}

static void				get_marks(t_command *c)
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

static void				get_command(t_parser *p, t_command *c)
{
	if ((read(p->fd_input, &(c->code), 1)) == -1)
		error("Command", errno);
	c->size += 1;
	c->name = g_op_tab[c->code - 1].name;
	c->typecode = 0;
	if (g_op_tab[c->code - 1].typecode)
	{
		if ((read(p->fd_input, &(c->typecode), 1)) == -1)
			error("Command", errno);
		c->size += 1;
	}
	c->tdirsize = g_op_tab[c->code - 1].tdirsize ? 2 : 4;
	c->argc = g_op_tab[c->code - 1].argc;
	get_argtypes(c);
	get_marks(c);
	get_arguments(p, c);
}

void					get_commands(t_parser *p)
{
	t_command			c;
	unsigned int		pos;

	pos = 0;
	while (pos < p->header.prog_size)
	{
		ft_bzero(&c, sizeof(t_command));
		get_command(p, &c);
		write_command(p, &c);
		pos += c.size;
	}
}
