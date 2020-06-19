/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sselusa <sselusa@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/17 09:51:53 by sselusa           #+#    #+#             */
/*   Updated: 2020/06/17 10:25:55 by sselusa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dasm.h"

static int				error(char *msg)
{
	ft_putendl(msg);
	return (1);
}

static void				usage(void)
{
	ft_printf("Usage:\n\t./dasm [file.cor]\n");
}

static unsigned char	filecheck(char *filename)
{
	int					fd;

	if ((fd = open(filename, O_RDONLY)) == -1)
		return (0);
	close(fd);
	if (!ft_strequ(ft_strrchr(filename, '.'), ".cor"))
		return (0);
	return (1);
}

int						main(int ac, char **av)
{
	if (ac == 2)
	{
		if (!filecheck(av[1]))
			return (error("Wrong binary!"));
		else
			parse(av[1]);
	}
	else
		usage();
}
