# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/07/15 21:26:09 by minkyeki          #+#    #+#              #
#    Updated: 2022/07/17 14:45:17 by minkyeki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			= minishell
CC				= cc
CFLAGS			= -Wall -Wextra -Werror
INCLUDE			= include

SRC_DIR			= src
LIBFT_DIR		= $(SRC_DIR)/libft/
MAIN_DIR		= $(SRC_DIR)/main/
LEXER_DIR		= $(SRC_DIR)/lexer/
PARSER_DIR		= $(SRC_DIR)/parser/
EXECUTER_DIR	= $(SRC_DIR)/executer/

# NOTE : Add Source files here
# ------------------------------------------------------ #
MAIN_SRC		= minishell helper
LEXER_SRC		= string iterator token scanner
PARSER_SRC		= parse_tree
EXECUTER_SRC	=
# ------------------------------------------------------ #

SRC = $(addsuffix .c, $(addprefix $(LEXER_DIR), $(LEXER_SRC))) \
	  $(addsuffix .c, $(addprefix $(PARSER_DIR), $(PARSER_SRC))) \
	  $(addsuffix .c, $(addprefix $(MAIN_DIR), $(MAIN_SRC)))

OBJ = $(SRC:c=o)

all: $(NAME)

# Colors
DEF_COLOR = \033[0;39m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m

$(NAME): $(OBJ)
	@make bonus -C $(LIBFT_DIR)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT_DIR)libft.a -lreadline
	@echo "$(RED)===========================================================================$(DEF_COLOR)"
	@echo "$(RED)|                                                                         |$(DEF_COLOR)"
	@echo "$(RED)|   Welcome to 42 minishell project.                                      |$(DEF_COLOR)"
	@echo "$(RED)|                                                                         |$(DEF_COLOR)"
	@echo "$(RED)|                                                                         |$(DEF_COLOR)"
	@echo "$(RED)|   ██╗     ███████╗███████╗    ███████╗██╗  ██╗███████╗██╗     ██╗       |$(DEF_COLOR)"
	@echo "$(RED)|   ██║     ██╔════╝██╔════╝    ██╔════╝██║  ██║██╔════╝██║     ██║       |$(DEF_COLOR)"
	@echo "$(RED)|   ██║     █████╗  █████╗      ███████╗███████║█████╗  ██║     ██║       |$(DEF_COLOR)"
	@echo "$(RED)|   ██║     ██╔══╝  ██╔══╝      ╚════██║██╔══██║██╔══╝  ██║     ██║       |$(DEF_COLOR)"
	@echo "$(RED)|   ███████╗███████╗███████╗    ███████║██║  ██║███████╗███████╗███████╗  |$(DEF_COLOR)"
	@echo "$(RED)|   ╚══════╝╚══════╝╚══════╝    ╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝  |$(DEF_COLOR)"                
	@echo "$(RED)|                                                                         |$(DEF_COLOR)"
	@echo "$(RED)|                                                                         |$(DEF_COLOR)"
	@echo "$(RED)|                                          .created by yehan & minkyeki   |$(DEF_COLOR)"
	@echo "$(RED)|                                                                         |$(DEF_COLOR)"
	@echo "$(RED)===========================================================================$(DEF_COLOR)"
	@echo "\n"

%.o: %.c
	@${CC} ${CFLAGS} -c $< -o $@
	@echo "$(RED)Compiling... \t$< $(DEF_COLOR)"

clean:
	@make clean -C $(LIBFT_DIR)
	@rm -f $(OBJ)
	@echo "$(RED)Minishell obj files has been deleted.$(DEF_COLOR)"

fclean:
	@make fclean -C $(LIBFT_DIR)
	@rm -f $(OBJ)
	@rm -f $(NAME)
	@echo "$(RED)Minishell archive files has been deleted.$(DEF_COLOR)"

re: fclean all
	@echo "$(RED)Cleaned and rebuilt LEE-SHELL.$(DEF_COLOR)"
