/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sselusa <sselusa@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/30 00:14:21 by sselusa           #+#    #+#             */
/*   Updated: 2020/06/30 00:27:07 by sselusa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dasm.h"

static void				get_t_reg(t_parser *p, t_command *c,
							unsigned int argno)
{
	if ((read(p->fd_input, &(c->arg[argno]), 1)) == -1)
		error("T_REG", errno);
	c->size += 1;
}

static void				get_t_dir(t_parser *p, t_command *c,
							unsigned int argno)
{
	if ((read(p->fd_input, &(c->arg[argno]), c->tdirsize)) == -1)
		error("T_DIR", errno);
	if (c->tdirsize == 4)
		c->arg[argno] = (int)reverse4(c->arg[argno]);
	else
		c->arg[argno] = (short)reverse2(c->arg[argno]);
	c->size += c->tdirsize;
}

static void				get_t_ind(t_parser *p, t_command *c,
							unsigned int argno)
{
	if ((read(p->fd_input, &(c->arg[argno]), 2)) == -1)
		error("T_IND", errno);
	c->arg[argno] = reverse2(c->arg[argno]);
	c->size += 2;
}

void					get_arguments(t_parser *p, t_command *c)
{
	unsigned int		argno;

	argno = 0;
	while (argno < c->argc)
	{
		if (c->argtype[argno] == T_REG)
			get_t_reg(p, c, argno);
		else if (c->argtype[argno] == T_DIR)
			get_t_dir(p, c, argno);
		else if (c->argtype[argno] == T_IND)
			get_t_ind(p, c, argno);
		argno++;
	}
}
