# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: minkyeki <minkyeki@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/12 14:06:52 by minkyeki          #+#    #+#              #
#    Updated: 2022/07/12 12:31:43 by minkyeki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 			= minishell
CC				= cc
# CFLAGS			= -Werror -Wextra -Wall -lreadline
CFLAGS			= -Werror -Wextra -Wall

# NOTE: Add source files here!
# ==========================================
SRC_FILES       = main
LEXER_SRC_FILES	= 
# ==========================================

LIBFT           = libft
# LEXER           = lexer
INCLUDE         = include
RM              = rm -f

SRC_DIR		    = src/
LEXER_SRC_DIR	= src/lexer
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
	@make -C $(LIBFT)
	mkdir $(OBJ_DIR)
	mv libft/libft.a ./$(OBJ_DIR)
	$(CC) $(CFLAGS) $^ $(OBJ_DIR)/libft.a -o $@
	@echo "$(BLUE)============================================$(DEF_COLOR)"
	@echo "$(BLUE)|        Minishell compile finished.       |$(DEF_COLOR)"
	@echo "$(BLUE)============================================$(DEF_COLOR)"

$(OBJ_DIR)%.o: $(SRC_DIR)%.c | $(LEXER_SRC_DIR) #| $(OBJ_MKDIR)
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $^ -o $@
	@echo "$(YELLOW)Compiling... \t$< $(DEF_COLOR)"

# $(OBJ_MKDIR):
	# @mkdir -p $(OBJ_DIR)

clean:
	@$(RM) -rf libft.a
	@make fclean -C $(LIBFT)
	@$(RM) -rf $(OBJ_DIR)
	@echo "$(BLUE)Minishell obj files has been deleted.$(DEF_COLOR)"

fclean:		clean
	@$(RM) -f $(NAME)
	@echo "$(BLUE)Minishell archive files has been deleted.$(DEF_COLOR)"

re:			fclean all
	@echo "$(GREEN)Cleaned and rebuilt Pipe-X.$(DEF_COLOR)"

.PHONY:		all bonus clean fclean re