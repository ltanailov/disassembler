/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bitmaps.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sselusa <sselusa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/29 22:47:15 by sselusa           #+#    #+#             */
/*   Updated: 2020/07/05 14:19:26 by sselusa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dasm.h"

unsigned int			reverse2(unsigned int x)
{
	x = (x & 0x00FF00FF) << 8 | (x & 0xFF00FF00) >> 8;
	return (x);
}

unsigned int			reverse4(unsigned int x)
{
	x = (x & 0x00FF00FF) << 8 | (x & 0xFF00FF00) >> 8;
	x = (x & 0x0000FFFF) << 16 | (x & 0xFFFF0000) >> 16;
	return (x);
}

t_arg_type				check_argtype(unsigned int argno,
							unsigned char typecode)
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
