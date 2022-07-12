/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 13:04:24 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/12 13:31:24 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCANNER_H
# define SCANNER_H

#include "iterator.h"

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

typedef enum e_id {
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
}	t_id;

typedef struct s_token {
	t_iterator	*src; // source of input

	t_id		id;
	char		*text; // pointer to string
	int			text_len; // length of string
}	t_token;




#endif /* SCANNER_H */