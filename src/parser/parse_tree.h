/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tree.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 14:22:29 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/17 16:02:33 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_TREE_H
# define PARSE_TREE_H

typedef struct s_tree	t_tree;

typedef struct s_tree {
	t_list		*token; // list of tokens
	t_list		*redirection;
	t_tree		*left;
	t_tree		*right;
}	t_tree;

t_tree *parse(t_list *tokens);
t_tree *parse_to_tree_recur(t_list *tokens);
t_list	*collect_redirection_node(t_list *tokens);
void	ft_lst_pop_one(t_list **node, t_list *pop_target);
t_list	*ft_lst_get_prev_node(t_list *node, t_list *target);
t_list	*find_top_priority_token(t_list *tokens);
t_list	*find_target_token(t_list *tokens, t_token_type _type);
t_tree	*new_tree_node(void);
void	delete_tree_node(t_tree *node);

#endif /* PARSE_H */
