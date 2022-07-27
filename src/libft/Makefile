# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/16 15:31:26 by minkyeki          #+#    #+#              #
#    Updated: 2022/07/27 19:10:13 by minkyeki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME            = libft.a

CC              = cc
CFLAGS          = -Werror -Wextra -Wall -I 
# -I option으로 헤더의 위치가 바뀌더라도 .c파일 내부에서 경로를 바꿀 필요 없음. 
INCLUDE         = include
SRC_DIR         = src/
OBJ_DIR         = obj/

RM              = rm -f
AR              = ar rcs

SRC_FILES       = ft_atoi ft_bzero ft_calloc ft_isalnum ft_isalpha \
                  ft_isascii ft_isdigit ft_isprint ft_itoa ft_memchr \
                  ft_memcmp ft_memcpy ft_memmove ft_memset ft_putchar_fd \
                  ft_putendl_fd ft_putnbr_fd ft_putstr_fd ft_split ft_strchr \
                  ft_strdup ft_striteri ft_strjoin ft_strlcat ft_strlcpy \
                  ft_strlen ft_strmapi ft_strncmp ft_strnstr ft_strrchr \
                  ft_strtrim ft_substr ft_tolower ft_toupper

SRC2_FILES      = ft_isspace ft_nbrlen ft_putnbr_uint_fd ft_nbrlen_uint \
                  ft_convert_nbr_base_malloc ft_nputstr_fd ft_atoi_base \
				  ft_strjoin_all ft_isinteger

SRC3_FILES		= get_next_line get_next_line_utils \
				  get_next_line_bonus get_next_line_utils_bonus

SRC4_FILES		= ft_printf ft_printf_hex ft_printf_info ft_printf_parse ft_printf_spec1 ft_printf_spec2

SRC5_FILES		= darray_create darray_modify darray_iterate \
				  vector

SRC_BONUS_FILES = ft_lstadd_back ft_lstadd_front ft_lstclear ft_lstdelone \
                  ft_lstiter ft_lstlast ft_lstmap ft_lstnew ft_lstsize

SRC             = $(addprefix $(SRC_DIR), $(addsuffix .c, $(SRC_FILES) $(SRC2_FILES) $(SRC3_FILES) $(SRC4_FILES) $(SRC5_FILES)))
OBJ             = $(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_FILES) $(SRC2_FILES) $(SRC3_FILES) $(SRC4_FILES) $(SRC5_FILES)))

SRC_BONUS       = $(addprefix $(SRC_DIR), $(addsuffix .c, $(SRC_BONUS_FILES)))
OBJ_BONUS       = $(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_BONUS_FILES)))

OBJ_MANDATORY   = ${OBJ} ${OBJ2}


# Keep makefile from relink
ifdef WITH_BONUS
     OBJ_RESULT = $(OBJ_MANDATORY) $(OBJ_BONUS)
else
     OBJ_RESULT = $(OBJ_MANDATORY)
endif

OBJ_MKDIR       = create_dir


# Colors
DEF_COLOR       = \033[0;39m
GRAY            = \033[0;90m
RED             = \033[0;91m
GREEN           = \033[0;92m
YELLOW          = \033[0;93m
BLUE            = \033[0;94m
MAGENTA         = \033[0;95m
CYAN            = \033[0;96m
WHITE           = \033[0;97m


# Rules 
all: $(NAME)

$(NAME): $(OBJ_RESULT)
	@$(AR) $(NAME) $^
	@echo "$(RED)===========================$(DEF_COLOR)"
	@echo "$(RED)| Libft compile finished. |$(DEF_COLOR)"
	@echo "$(RED)===========================$(DEF_COLOR)"

$(OBJ_DIR)%.o: $(SRC_DIR)%.c | $(OBJ_MKDIR)
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@
	@echo "$(RED)Compiling... \t$< $(DEF_COLOR)"

$(OBJ_MKDIR):
	@mkdir -p $(OBJ_DIR)

bonus:
	@make WITH_BONUS=1

clean:
	@${RM} -r ${OBJ_DIR}
	@echo "$(RED)Libft obj files has been deleted.$(DEF_COLOR)"

fclean: clean
	@${RM} ${NAME}
	@echo "$(RED)Libft archive files has been deleted.$(DEF_COLOR)"

re: fclean all
	@echo "$(RED)Cleaned and rebuilt everything.$(DEF_COLOR)"

.PHONY: all clean fclean re bonus
