# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/07/15 21:26:09 by minkyeki          #+#    #+#              #
#    Updated: 2022/07/25 22:07:42 by minkyeki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			= minishell
CC				= cc
CFLAGS			= -Wall -Wextra -Werror
INCLUDE			= include

# NOTE : Add Source directory here
# ------------------------------------------------------ #
SRC_DIR			= src

MAIN_DIR		= $(SRC_DIR)/main/
LEXER_DIR		= $(SRC_DIR)/lexer/
PARSER_DIR		= $(SRC_DIR)/parser/
EXECUTER_DIR	= $(SRC_DIR)/executer/
BUILTIN_DIR		= $(SRC_DIR)/builtin/

LIBFT_DIR		= $(SRC_DIR)/libft/
STRING_DIR		= $(SRC_DIR)/string/
ITERATOR_DIR	= $(SRC_DIR)/iterator/

# NOTE : Add Source files here
# ------------------------------------------------------ #
MAIN_SRC		= minishell prompt helper

LEXER_SRC		= token_create token_modify \
				  scanner_main scanner_create scanner_function_ptr \
				  scanner_helper scanner_get_token_1 scanner_get_token_2

PARSER_SRC		= parse_tree parse_tree_utils
EXECUTER_SRC	= executer token_expand get_full_path redirection

BUILTIN_SRC		= environ environ_utils environ_helper cd echo pwd exit
STRING_SRC		= string_create string_modify string_utils
ITERATOR_SRC	= iterator_create iterator_modify

# NOTE : Add to SRC here
# ------------------------------------------------------ #
SRC = $(addsuffix .c, $(addprefix $(MAIN_DIR), $(MAIN_SRC))) \
	  $(addsuffix .c, $(addprefix $(LEXER_DIR), $(LEXER_SRC))) \
	  $(addsuffix .c, $(addprefix $(PARSER_DIR), $(PARSER_SRC))) \
	  $(addsuffix .c, $(addprefix $(EXECUTER_DIR), $(EXECUTER_SRC))) \
	  $(addsuffix .c, $(addprefix $(BUILTIN_DIR), $(BUILTIN_SRC))) \
	  $(addsuffix .c, $(addprefix $(STRING_DIR), $(STRING_SRC))) \
	  $(addsuffix .c, $(addprefix $(ITERATOR_DIR), $(ITERATOR_SRC)))

# ------------------------------------------------------ #

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

	# @echo "$(RED)===========================================================================$(DEF_COLOR)"
	# @echo "$(RED)|                                                                         |$(DEF_COLOR)"
	# @echo "$(RED)|   Welcome to 42 minishell project.                                      |$(DEF_COLOR)"
	# @echo "$(RED)|                                                                         |$(DEF_COLOR)"
	# @echo "$(RED)|                                                                         |$(DEF_COLOR)"
	# @echo "$(RED)|   ██╗     ███████╗███████╗    ███████╗██╗  ██╗███████╗██╗     ██╗       |$(DEF_COLOR)"
	# @echo "$(RED)|   ██║     ██╔════╝██╔════╝    ██╔════╝██║  ██║██╔════╝██║     ██║       |$(DEF_COLOR)"
	# @echo "$(RED)|   ██║     █████╗  █████╗      ███████╗███████║█████╗  ██║     ██║       |$(DEF_COLOR)"
	# @echo "$(RED)|   ██║     ██╔══╝  ██╔══╝      ╚════██║██╔══██║██╔══╝  ██║     ██║       |$(DEF_COLOR)"
	# @echo "$(RED)|   ███████╗███████╗███████╗    ███████║██║  ██║███████╗███████╗███████╗  |$(DEF_COLOR)"
	# @echo "$(RED)|   ╚══════╝╚══════╝╚══════╝    ╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝  |$(DEF_COLOR)"
	# @echo "$(RED)|                                                                         |$(DEF_COLOR)"
	# @echo "$(RED)|                                                                         |$(DEF_COLOR)"
	# @echo "$(RED)|                                          .created by yehan & minkyeki   |$(DEF_COLOR)"
	# @echo "$(RED)|                                                                         |$(DEF_COLOR)"
	# @echo "$(RED)===========================================================================$(DEF_COLOR)"
	# @echo "\n"


$(NAME): $(OBJ)
	@make bonus -C $(LIBFT_DIR)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT_DIR)libft.a -lreadline
	
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
