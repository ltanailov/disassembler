/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sselusa <sselusa@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/30 00:27:02 by sselusa           #+#    #+#             */
/*   Updated: 2020/06/30 00:28:16 by sselusa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dasm.h"

void					write_header(t_parser *p)
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

void					write_command(t_parser *p, t_command *c)
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
