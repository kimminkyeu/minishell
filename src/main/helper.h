/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 22:22:10 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/24 20:42:33 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HELPER_H
# define HELPER_H

# include <stdio.h>
# include "../../include/lexer.h"
# include "../../include/parse.h"

void		print_error(char *messege, char *err_char);

const char	*get_token_type(t_token_type type);
void		print_tokens(t_list *tokens);
void		print_tree(t_tree *root);
void		print_tree_node(t_list *token);

#endif /* HELPER_H */
