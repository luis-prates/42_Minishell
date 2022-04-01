/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_memory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rramos <rramos@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 18:03:09 by rramos            #+#    #+#             */
/*   Updated: 2022/03/30 20:37:11 by rramos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_memory(void **memory_pointer)
{
	if (*memory_pointer != NULL)
	{
		free(*memory_pointer);
		*memory_pointer = NULL;
	}
}
