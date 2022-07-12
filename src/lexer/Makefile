# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: minkyeki <minkyeki@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/07/12 12:32:38 by minkyeki          #+#    #+#              #
#    Updated: 2022/07/12 12:39:54 by minkyeki         ###   ########.fr        #
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

NAME 			= lexer.o
CC				= cc
CFLAGS			= -Werror -Wextra -Wall

# NOTE: Add souce files here!
# ==========================================
SRC_FILES			= 
# ==========================================

LIBFT		= libft
RM			= rm -f

SRC 			= $(addprefix $(SRC_DIR), $(addsuffix .c, $(SRC_FILES)))
OBJ 			= $(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_FILES)))
OBJ_MANDATORY	= $(OBJ)

SRC_BONUS 		= $(addprefix $(SRC_DIR), $(addsuffix .c, $(SRC_FILES_BONUS)))
OBJ_BONUS 		= $(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_FILES_BONUS)))

SRC_DIR		= 
OBJ_DIR		= obj/

# Keep makefile from relink
ifdef BONUS
     OBJ_RESULT = $(OBJ_BONUS)
else
     OBJ_RESULT = $(OBJ_MANDATORY)
endif

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

$(NAME): $(OBJ_RESULT)
	@make bonus -C $(LIBFT)
	mv libft/libft.a ./$(OBJ_DIR)
	$(CC) $(CFLAGS) $^ $(OBJ_DIR)libft.a -o $@
	@echo "$(YELLOW)============================================$(DEF_COLOR)"
	@echo "$(YELLOW)|         Lexer compile finished.          |$(DEF_COLOR)"
	@echo "$(YELLOW)============================================$(DEF_COLOR)"

$(OBJ_DIR)%.o: $(SRC_DIR)%.c | $(OBJ_MKDIR)
	$(CC) $(CFLAGS) -c $^ -o $@
	@echo "$(YELLOW)Compiling... \t$< $(DEF_COLOR)"

$(OBJ_MKDIR):
	@mkdir -p $(OBJ_DIR)

clean:
	@$(RM) -rf libft.a
	@make fclean -C $(LIBFT)
	@$(RM) -rf $(OBJ_DIR)
	@echo "$(BLUE)Lexer obj files has been deleted.$(DEF_COLOR)"

fclean:		clean
	@$(RM) -f $(NAME)
	@echo "$(BLUE)Lexer archive files has been deleted.$(DEF_COLOR)"

re:			fclean all
	@echo "$(GREEN)Cleaned and rebuilt Lexer.$(DEF_COLOR)"

.PHONY:		all bonus clean fclean re