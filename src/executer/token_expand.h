/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expand.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 15:47:13 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/22 16:03:15 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_EXPAND_H
# define TOKEN_EXPAND_H

#include "../executer/executer.h"

char	**get_arglist(t_list *token);
void	expand_dollar_sign(t_token *tok, t_shell_config *config);
int		expand_token(t_list *tokens, t_shell_config *config);

#endif /* TOKEN_EXPAND_H */
