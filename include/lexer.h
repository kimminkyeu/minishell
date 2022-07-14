/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 14:25:00 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/14 14:37:18 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

#include "./libft.h"
#include "../src/lexer/scanner.h"
#include "../src/lexer/token.h"
//

/* @ Returns t_token list.
 * @ NOTE : doesn't check synstax error!
 * */
extern t_list	*tokenize(char *line);

/* @ Check syntax.
 *
 * extern void	check_syntax(t_list *token_list)
 * */

#endif /* LEXER_H */
