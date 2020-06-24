/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dasm.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sselusa <sselusa@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/17 09:51:52 by sselusa           #+#    #+#             */
/*   Updated: 2020/06/24 22:05:24 by sselusa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DASM_H
# define DASM_H

# include "libft.h"
# include "op.h"

# define DIV_SIZE 4

int						parse(char *filename);

/*
**	Структура t_op (операция):
**		name		- имя
**		argc		- количество аргументов
**		argtype		- типы аргументов
**		code		- код (номер)
**		cycles		- циклы до исполнения
**		comment		- описание
**		typecode	- флаг необходимости задания типов аргументов
**						(0 - нет необходимости)
**						(1 - необходимо задать)
**		tdirsize	- флаг размера T_DIR
**						(0 - 4 байта)
**						(1 - 2 байта)
*/
typedef struct          s_op
{
	const char          *name;
	int                 argc;
	t_arg_type			argtype[3];
	unsigned char       code;
	unsigned int		cycles;
	const char			*comment;
	unsigned char		typecode;
	unsigned char		tdirsize;
}                       t_op;

typedef struct			s_parser
{
	int					fd_input;
	char				*input_filename;
	int					fd_output;
	char				*output_filename;
	header_t			header;
	mode_t				openmode;
}						t_parser;

#endif
