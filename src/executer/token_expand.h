/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expand.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: han-yeseul <han-yeseul@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 15:47:13 by minkyeki          #+#    #+#             */
/*   Updated: 2022/08/01 20:03:58 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_EXPAND_H
# define TOKEN_EXPAND_H

# include "../executer/executer.h"

/* Pattern for wildcard expansion */
typedef struct s_pattern {
	t_string	*prefix;
	t_string	*suffix;
	t_string	*remainder;
	t_string	*path;
}	t_pattern;

int		expand_wildcard_glob(t_list *tokens, t_shell_config *config);

char	**get_arglist(t_list *token);

int		expand_token(t_list *tokens, t_shell_config *config);

char	**get_cmd_argv(t_list *token);

void	expand_dollar_sign(t_string *str, t_iterator *iter, \
		bool *is_dollar_expanded, t_shell_config *config);

int		expand_double_quote(t_string *str, t_iterator *iter, \
		bool *is_dollar_expanded, t_shell_config *config);

int		expand_single_quote(t_string *str, t_iterator *iter);

#endif /* TOKEN_EXPAND_H */
