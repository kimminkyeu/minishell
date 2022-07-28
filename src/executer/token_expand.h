/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expand.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: han-yeseul <han-yeseul@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 15:47:13 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/28 19:22:09 by han-yeseul       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_EXPAND_H
# define TOKEN_EXPAND_H

# include "../executer/executer.h"

char	**get_arglist(t_list *token);
int		expand_token(t_list *tokens, t_shell_config *config);

char	**get_cmd_argv(t_list *token);
void	expand_dollar_sign(t_string *str, t_iterator *iter, \
			t_shell_config *config);
int		expand_double_quote(t_string *str, t_iterator *iter, \
			t_shell_config *config);
int		expand_single_quote(t_string *str, t_iterator *iter);

#endif /* TOKEN_EXPAND_H */
