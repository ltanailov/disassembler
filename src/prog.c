/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prog.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sselusa <sselusa@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/24 22:51:40 by sselusa           #+#    #+#             */
/*   Updated: 2020/06/25 00:36:13 by sselusa          ###   ########.fr       */
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

static void				set_argtypes(t_arg_type *args, unsigned char typecode)
{
	args[0] = check_argtype(1, typecode);
	args[1] = check_argtype(2, typecode);
	args[2] = check_argtype(3, typecode);
}

static void				print_binary(unsigned char byte)
{
	int					i;

	i = 0;
	ft_putstr("BINARY: 0b");
	while (i < 8)
	{
		if (byte & (1 << (7 - i)))
			ft_putchar('1');
		else
			ft_putchar('0');
		i++;
	}
	ft_putchar('\n');
}

static void				print_argument(t_parser *p, unsigned char code, t_arg_type type)
{
	unsigned int		mem;
	unsigned int		tdirsize;

	mem = 0;
	if (type == T_REG)
	{
		ft_putchar_fd('r', p->fd_output);
		read(p->fd_input, &mem, 1);
		ft_putnbr_fd((int)mem, p->fd_output);
		ft_printf("r%d", mem);
	}
	else if (type == T_DIR)
	{
		ft_putchar_fd('%', p->fd_output);
		tdirsize = op_tab[code - 1].tdirsize ? 2 : 4;
		read(p->fd_input, &mem, tdirsize);
		mem = tdirsize == 4 ? reverse4(mem) : mem;
		ft_putnbr_fd((int)mem, p->fd_output);
		ft_printf("%%%d", mem);
	}
	else if (type == T_IND)
	{
		read(p->fd_input, &mem, 2);
		ft_putnbr_fd((int)mem, p->fd_output);
		ft_printf("%d", mem);
	}
}

void					prog(t_parser *p)
{
	unsigned char		code;
	unsigned char		typecode;
	t_arg_type			args[3];

	ft_bzero(args, sizeof(unsigned char) * 3);
	read(p->fd_input, &code, 1);
	ft_printf("FIRST COMMAND IS: %s\n", op_tab[code - 1].name);
	ft_putstr_fd(op_tab[code - 1].name, p->fd_output);
	ft_putchar_fd(' ', p->fd_output);
	typecode = 0;
	if (op_tab[code - 1].typecode)
	{
		read(p->fd_input, &typecode, 1);
		ft_printf("TYPECODE: %#x\n", typecode);
		print_binary(typecode);
		set_argtypes(args, typecode);
		ft_printf("TYPES: %d | %d | %d\n", args[0], args[1], args[2]);
	}
	else
	{
		args[0] = op_tab[code - 1].argtype[0];
		args[1] = op_tab[code - 1].argtype[1];
		args[2] = op_tab[code - 1].argtype[2];
	}
	ft_printf("%s ", op_tab[code - 1].name);
	print_argument(p, code, args[0]);
	if (args[1])
	{
		ft_printf(",");
		ft_putstr_fd(",", p->fd_output);
	}
	else
	{
		ft_printf("\n");
		ft_putchar_fd('\n', p->fd_output);
	}
	print_argument(p, code, args[1]);
	if (args[2])
	{
		ft_printf(",");
		ft_putstr_fd(",", p->fd_output);
	}
	print_argument(p, code, args[2]);
	ft_printf("\n");
	ft_putchar_fd('\n', p->fd_output);

	// while (pos < p->header.prog_size)
	// {
	//     // Read code
	//     // Read typecode if necessary
	//     // Read arguments
	//     // Write command
	// }
}
