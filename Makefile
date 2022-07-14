# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: minkyeki <minkyeki@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/12 14:06:52 by minkyeki          #+#    #+#              #
#    Updated: 2022/07/14 20:19:11 by minkyeki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 			= minishell
CC				= cc
# CFLAGS			= -Werror -Wextra -Wall -lreadline
CFLAGS			= -Werror -Wextra -Wall

# NOTE: Add source files here!
# ==========================================
SRC_FILES       = minishell
# ==========================================

LIBFT           = src/libft
LEXER           = src/lexer
INCLUDE         = include
RM              = rm -f

SRC_DIR		    = src/
OBJ_DIR		    = obj/


SRC             = $(addprefix $(SRC_DIR), $(addsuffix .c, $(SRC_FILES)))
LEXER_SRC		= $(addprefix $(LEXER_SRC_DIR), $(addsuffix .c, $(LEXER_SRC_FILES)))

OBJ             = $(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_FILES)))


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

OBJ_MKDIR		=	create_dir

all: $(NAME)

$(NAME): $(OBJ)
	make bonus -C $(LIBFT)
	make -C $(LEXER)
	$(CC) $(CFLAGS) $^ $(LIBFT)/libft.a $(LEXER)/lexer.a -lreadline -o $@
	@echo "$(BLUE)--------------------------------------------------------------------------$(DEF_COLOR)"
	@echo "$(BLUE)|                                                                        |$(DEF_COLOR)"
	@echo "$(BLUE)|  ██╗     ███████╗███████╗    ███████╗██╗  ██╗███████╗██╗     ██╗       |$(DEF_COLOR)"
	@echo "$(BLUE)|  ██║     ██╔════╝██╔════╝    ██╔════╝██║  ██║██╔════╝██║     ██║       |$(DEF_COLOR)"
	@echo "$(BLUE)|  ██║     █████╗  █████╗      ███████╗███████║█████╗  ██║     ██║       |$(DEF_COLOR)"
	@echo "$(BLUE)|  ██║     ██╔══╝  ██╔══╝      ╚════██║██╔══██║██╔══╝  ██║     ██║       |$(DEF_COLOR)"
	@echo "$(BLUE)|  ███████╗███████╗███████╗    ███████║██║  ██║███████╗███████╗███████╗  |$(DEF_COLOR)"
	@echo "$(BLUE)|  ╚══════╝╚══════╝╚══════╝    ╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝  |$(DEF_COLOR)"                
	@echo "$(BLUE)|                                                                        |$(DEF_COLOR)"
	@echo "$(BLUE)--------------------------------------------------------------------------$(DEF_COLOR)"

$(OBJ_DIR)%.o: $(SRC_DIR)%.c | $(OBJ_MKDIR)
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $^ -o $@
	@echo "$(YELLOW)Compiling... \t$< $(DEF_COLOR)"

$(OBJ_MKDIR):
	@mkdir -p $(OBJ_DIR)

clean:
	@make fclean -C $(LIBFT)
	@make fclean -C $(LEXER)
	@$(RM) -rf $(OBJ_DIR)
	@echo "$(BLUE)Minishell obj files has been deleted.$(DEF_COLOR)"

fclean:		clean
	@$(RM) -f $(NAME)
	@echo "$(BLUE)Minishell archive files has been deleted.$(DEF_COLOR)"

re:			fclean all
	@echo "$(GREEN)Cleaned and rebuilt Pipe-X.$(DEF_COLOR)"

.PHONY:		all bonus clean fclean re
