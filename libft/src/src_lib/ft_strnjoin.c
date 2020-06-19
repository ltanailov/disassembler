/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnjoin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldonnor- <ldonnor-@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/13 18:07:24 by lshellie          #+#    #+#             */
/*   Updated: 2020/03/04 12:35:32 by ldonnor-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnjoin(char const *s1, char const *s2, int n)
{
	char	*ret;
	size_t	len;
	size_t	len1;

	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len = len1 + n;
	ret = ft_strnew(len);
	if (!ret)
		return (NULL);
	ft_strcat(ret, s1);
	while (len1 < len)
	{
		if (*s2)
			ret[len1] = *s2;
		else
			ret[len1] = 0;
		len1++;
		s2++;
	}
	return (ret);
}
