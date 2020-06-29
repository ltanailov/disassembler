/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sselusa <sselusa@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/29 23:52:52 by sselusa           #+#    #+#             */
/*   Updated: 2020/06/29 23:59:45 by sselusa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dasm.h"

static void				get_magic(t_parser *p)
{
	if ((read(p->fd_input, &(p->header.magic),
		sizeof(unsigned int))) == -1)
		error("Magic", errno);
	p->header.magic = reverse4(p->header.magic);
}

static void				get_prog_name(t_parser *p)
{
	if ((read(p->fd_input, p->header.prog_name,
		PROG_NAME_LENGTH)) == -1)
		error("Program name", errno);
	p->header.prog_name[PROG_NAME_LENGTH] = '\0';
}

static void				get_prog_size(t_parser *p)
{
	if ((read(p->fd_input, &(p->header.prog_size),
		sizeof(unsigned int))) == -1)
		error("Program size", errno);
	p->header.prog_size = reverse4(p->header.prog_size);
}

static void				get_comment(t_parser *p)
{
	if ((read(p->fd_input, p->header.comment,
		COMMENT_LENGTH)) == -1)
		error("Comment", errno);
	p->header.comment[COMMENT_LENGTH] = '\0';
}

void					get_header(t_parser *p)
{
	get_magic(p);
	get_prog_name(p);
	skip_divider(p);
	get_prog_size(p);
	get_comment(p);
	skip_divider(p);
}
