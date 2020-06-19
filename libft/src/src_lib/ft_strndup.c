/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldonnor- <ldonnor-@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/13 18:02:25 by lshellie          #+#    #+#             */
/*   Updated: 2020/03/04 11:39:39 by ldonnor-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s1, int n)
{
	char	*str;
	int		j;

	j = 0;
	if (!(str = (char *)malloc(sizeof(char) * (n + 1))))
		return (0);
	str[n] = 0;
	while (j < n)
	{
		if (s1[j])
			str[j] = s1[j];
		else
			str[j] = 0;
		++j;
	}
	return (str);
}
