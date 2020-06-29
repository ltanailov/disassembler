/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sselusa <sselusa@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/17 09:51:53 by sselusa           #+#    #+#             */
/*   Updated: 2020/06/30 00:30:33 by sselusa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dasm.h"

void					error(char *msg, int code)
{
	errno = code;
	perror(msg);
	exit(errno);
}

void					skip_divider(t_parser *p)
{
	if ((lseek(p->fd_input, DIV_SIZE, SEEK_CUR)) == -1)
		error("Divider", errno);
}

static void				fname_check(char *fname)
{
	if (!ft_strequ(ft_strrchr(fname, '.'), ".cor"))
		error("Input file", EINVAL);
}

int						main(int ac, char **av)
{
	if (ac == 2)
	{
		fname_check(av[1]);
		parse(av[1]);
	}
	else
		ft_printf("Usage:\n\t./dasm [file.cor]\n");
	return (EXIT_SUCCESS);
}
