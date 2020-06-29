/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dasm.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sselusa <sselusa@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/17 09:51:52 by sselusa           #+#    #+#             */
/*   Updated: 2020/06/29 22:38:43 by sselusa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DASM_H
# define DASM_H

# include "libft.h"
# include "op.h"

# define DIV_SIZE 4
# define REGISTRY_CHAR 'r'

typedef struct			s_op
{
	const char			*name;
	unsigned int		argc;
	t_arg_type			argtype[3];
	unsigned char		code;
	unsigned int		cycles;
	const char			*comment;
	unsigned char		typecode;
	unsigned char		tdirsize;
}						t_op;

/*
**	Таблица операций op_tab[ ]:
**		name		- имя
**		argc		- количество аргументов
**		argtype		- типы аргументов
**		code		- код (номер)
**		cycles		- циклы до исполнения
**		comment		- описание
**		typecode	- флаг необходимости задания типов аргументов
**						(0 - нет необходимости / 1 - необходимо задать)
**		tdirsize	- флаг размера T_DIR
**						(0 - 4 байта / 1 - 2 байта)
*/
t_op					op_tab[17];

typedef struct			s_parser
{
	int					fd_input;
	char				*input_filename;
	int					fd_output;
	char				*output_filename;
	header_t			header;
	mode_t				openmode;
}						t_parser;

typedef struct			s_command
{
	unsigned char		code;
	const char			*name;
	unsigned char		typecode;
	unsigned int		tdirsize;
	unsigned int		argc;
	t_arg_type			argtype[3];
	char				mark[3];
	int					arg[3];
	unsigned int		size;
}						t_command;

int						parse(char *filename);

#endif
