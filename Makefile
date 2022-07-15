# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/07/15 21:26:09 by minkyeki          #+#    #+#              #
#    Updated: 2022/07/15 21:51:14 by minkyeki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= minishell
CC			= cc
CFLAGS		= -Wall -Wextra -Werror
INCLUDE		= include
SRC_DIR		= src

LIBFT_DIR	= $(SRC_DIR)/libft/
MAIN_DIR	= $(SRC_DIR)/main/
LEXER_DIR	= $(SRC_DIR)/lexer/

# NOTE : Add Source files here
# ------------------------------------------------------ #
MAIN_SRC	= minishell
LEXER_SRC	= string iterator token scanner
# ------------------------------------------------------ #

SRC = $(addsuffix .c, $(addprefix $(LEXER_DIR), $(LEXER_SRC))) \
	  $(addsuffix .c, $(addprefix $(MAIN_DIR), $(MAIN_SRC))) \

OBJ = $(SRC:c=o)

all: $(NAME)

$(NAME): $(OBJ)
	make bonus -C $(LIBFT_DIR)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT_DIR)libft.a -lreadline

%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@

clean:
	make clean -C $(LIBFT_DIR)
	rm -f $(OBJ)

fclean:
	make fclean -C $(LIBFT_DIR)
	rm -f $(OBJ)
	rm -f $(NAME)

re: fclean all
