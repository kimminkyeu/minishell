# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: minkyeki <minkyeki@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/07/12 12:32:38 by minkyeki          #+#    #+#              #
#    Updated: 2022/07/14 15:01:53 by minkyeki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/12 14:06:52 by minkyeki          #+#    #+#              #
#    Updated: 2022/05/17 16:48:02 by minkyeki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 			= parser.a
CC				= cc
CFLAGS			= -Werror -Wextra -Wall

# NOTE: Add souce files here!
# ==========================================
SRC_FILES			= 
# ==========================================

LIBFT		= ../libft
RM			= rm -f
AR          = ar rcs

SRC 			= $(addprefix $(SRC_DIR), $(addsuffix .c, $(SRC_FILES)))
OBJ 			= $(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_FILES)))

SRC_DIR		= 
OBJ_DIR		= obj/

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
	@make bonus -C $(LIBFT)
	$(AR) $(NAME) $^ $(LIBFT)/libft.a
	@echo "$(YELLOW)--------------------------------------------$(DEF_COLOR)"
	@echo "$(YELLOW)|         Lexer compile finished.          |$(DEF_COLOR)"
	@echo "$(YELLOW)--------------------------------------------$(DEF_COLOR)"

$(OBJ_DIR)%.o: $(SRC_DIR)%.c | $(OBJ_MKDIR)
	$(CC) $(CFLAGS) -c $^ -o $@
	@echo "$(YELLOW)Compiling... \t$< $(DEF_COLOR)"

$(OBJ_MKDIR):
	@mkdir -p $(OBJ_DIR)

clean:
	@$(RM) -rf $(OBJ_DIR)
	@echo "$(BLUE)Lexer obj files has been deleted.$(DEF_COLOR)"

fclean:		clean
	@$(RM) -f $(NAME)
	@echo "$(BLUE)Lexer archive files has been deleted.$(DEF_COLOR)"

re:			fclean all
	@echo "$(GREEN)Cleaned and rebuilt Lexer.$(DEF_COLOR)"

.PHONY:		all bonus clean fclean re
