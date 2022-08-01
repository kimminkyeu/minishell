/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expand.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yehan <yehan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 15:47:13 by minkyeki          #+#    #+#             */
/*   Updated: 2022/08/01 15:06:06 by yehan            ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_EXPAND_H
# define TOKEN_EXPAND_H

# include "../executer/executer.h"

int		expand_wildcard_glob(t_list *tokens, t_shell_config *config);

char	**get_arglist(t_list *token);

int		expand_token(t_list *tokens, t_shell_config *config);

char	**get_cmd_argv(t_list *token);

int		expand_dollar_sign(t_string *str, t_iterator *iter, \
		t_shell_config *config, t_token_type token_type);

int		expand_double_quote(t_string *str, t_iterator *iter, \
		bool *is_dollar_expanded, t_shell_config *config);

int		expand_single_quote(t_string *str, t_iterator *iter);

#endif /* TOKEN_EXPAND_H */
