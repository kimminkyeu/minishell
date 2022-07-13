/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 13:04:24 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/12 21:23:47 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCANNER_H
# define SCANNER_H

# include "iterator.h"

typedef enum e_state {
	E_PARSE_START,		// default state
	E_PARSE_END,
	E_PARSE_OPTION,		// ls => -al
	E_PARSE_ARG,		// double or signle quote + CMD_Argument
	E_PARSE_CMD,
	// E_PARSE_WHITESPACE,
	E_PARSE_ERROR,
	// ... Add accordingly
}	t_state;

typedef enum e_token_type {
	E_SPACE, 			// isspace()
	E_CMD,				// simple_command
	E_ARG,				// argument -> ex) echo [hello] --> hello = argument

	E_SINGLE_QUOTE, 	//  ['']
	E_DOUBLE_QUOTE,		//  [""]

	E_PIPE,				//	[|]
	E_REDIRECT,			//	[< << > >>]

	E_SEMICOLON,		//	[;]
	E_DOUBLE_AMPERSAND, //	[&&]
	E_DOUBLE_PIPE, 		//  [||]
	E_VAR,				//	[$VAR]
					
	E_EOF				//  [ EOF special token like NULL ]
}	t_token_type;

typedef struct s_token {
	t_token_type	type;
	t_iterator		*src; // source of input for token data access
	int				text_len;	// length of string
	char			*text;		// pointer to string
}	t_token;

/* add data to t_vector *tokens */
void	tokenize(char *str, t_vector *tokens);

// void	free_token(t_token *token);
#endif /* SCANNER_H */
