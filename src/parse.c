/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sselusa <sselusa@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/17 10:19:36 by sselusa           #+#    #+#             */
/*   Updated: 2020/06/30 00:34:57 by sselusa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dasm.h"

static void				open_output(t_parser *p)
{
	char				*path;
	size_t				len;

	len = (size_t)(ft_strrchr(p->input_filename, '.') - p->input_filename);
	if (!(path = ft_strsub(p->input_filename, 0, len)))
		error("Output filename", ENOMEM);
	if (!(p->output_filename = ft_strjoin(path, OUTPUT_EXTENSION)))
		error("Output filename", ENOMEM);
	free(path);
	if ((p->fd_output = open(p->output_filename,
		O_WRONLY | O_CREAT | O_TRUNC, p->openmode)) == -1)
		error("Output file", ENOENT);
}

void					parse(char *filename)
{
	t_parser			p;

	ft_bzero(&p, sizeof(t_parser));
	p.input_filename = filename;
	p.openmode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	if ((p.fd_input = open(p.input_filename, O_RDONLY)) == -1)
		error("Input file", ENOENT);
	open_output(&p);
	get_header(&p);
	write_header(&p);
	get_commands(&p);
	close(p.fd_input);
	close(p.fd_output);
	ft_printf("Disassembly done! [%s] -> [%s]\n",
		p.input_filename, p.output_filename);
	free(p.output_filename);
}
