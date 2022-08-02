/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yehan <yehan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 16:22:56 by minkyeki          #+#    #+#             */
/*   Updated: 2022/08/02 14:10:41 by yehan            ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_H
# define HEREDOC_H

# include "../lexer/token.h"

void	expand_quote(t_string *str, t_iterator *iter, char quote_type);
bool	is_limiter(const char *line, const char *limiter);
char	*readline_prompt_heredoc(void);
int		set_heredoc(t_list *tokens);

#endif /* HEREDOC_H */
