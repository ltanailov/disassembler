/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dasm.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sselusa <sselusa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/17 09:51:52 by sselusa           #+#    #+#             */
/*   Updated: 2020/07/05 14:24:28 by sselusa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DASM_H
# define DASM_H

# include "libft.h"
# include "op.h"
# include <errno.h>

# define DIV_SIZE 4
# define REGISTRY_CHAR 'r'
# define OUTPUT_EXTENSION ".s"

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
t_op					g_op_tab[17];

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

typedef struct			s_parser
{
	int					fd_input;
	char				*input_filename;
	int					fd_output;
	char				*output_filename;
	mode_t				openmode;
	header_t			header;
}						t_parser;

void					error(char *msg, int code);
void					skip_divider(t_parser *p);

void					parse(char *filename);

void					get_header(t_parser *p);
void					get_commands(t_parser *p);
void					get_arguments(t_parser *p, t_command *c);

void					write_header(t_parser *p);
void					write_command(t_parser *p, t_command *c);

unsigned int			reverse2(unsigned int x);
unsigned int			reverse4(unsigned int x);
t_arg_type				check_argtype(unsigned int argno,
							unsigned char typecode);

#endif
