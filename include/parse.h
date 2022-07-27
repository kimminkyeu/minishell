/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 14:22:29 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/27 14:07:37 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

# include "../src/parser/parse_tree.h"

/* @ Convert tokens lists to Abstract Syntax tree 
 *
 * NOTE : must use 'delete_tree_node()' while traversing each node!
 *
 * */
extern t_tree	*parse(t_list *tokens);

#endif /* PARSE_H */
