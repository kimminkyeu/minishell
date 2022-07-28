/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: han-yeseul <han-yeseul@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/28 18:56:29 by han-yeseul        #+#    #+#             */
/*   Updated: 2022/07/28 18:56:34 by han-yeseul       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTION_H
# define REDIRECTION_H

# include "../main/minishell.h"
# include "../lexer/token.h"

char	*expand_line_each(char *line, t_shell_config *config);
void	expand_file(t_token *tok, t_shell_config *config);

#endif/* REDIRECTION_H */
