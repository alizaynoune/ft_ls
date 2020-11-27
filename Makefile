# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/10/18 12:00:15 by alzaynou          #+#    #+#              #
#    Updated: 2020/11/24 20:48:30 by alzaynou         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =			ft_ls
GCC =			gcc -Wall -Werror -Wextra
LIB =			-lftdprintf
PATH_LIB =		ft_dprintf/
PATH_INC_LIB =	ft_dprintf/includes/
PATH_INC =		includes/
PATH_SRC =		src/
PATH_OBJ =		objs/
INC =			$(PATH_INC)ft_ls.h

#*
#** Src files
#*

SRC =	dirs.c					\
		free.c					\
		free_extended.c			\
		init.c					\
		init_extended.c			\
		length.c				\
		main.c					\
		parsing.c				\
		parsing_arg.c			\
		printing.c				\
		printing_extended.c		\
		push.c					\
		sort.c					\
		tools.c					\
		tools_helper.c

OBJS =  $(addprefix $(PATH_OBJ), $(SRC:.c=.o))

#*
#** Colors
#*


DEF = 		\x1B[1;0m
RED = 		\x1B[1;31m
PURPLE =	\x1B[1;34m
BLUE = 		\x1B[1;96m
GREEN = 	\x1B[1;32m
SILVER = 	\x1B[1;90m
YELLOW = 	\x1B[1;33m

#*
#** Functions
#*

define compile_obj
@mkdir -p $(PATH_OBJ)
@$(GCC) -c $1 -o $2 -I $(PATH_INC) -I $(PATH_INC_LIB)
@printf "$(SILVER)[$(PURPLE)Object file$(BLUE) $(notdir $2)$(SILVER)] $(GREEN)Created.$(DEF)\n"
endef

define compile_binary
@$(GCC) $(OBJS) -L $(PATH_LIB) $(LIB) -o $(NAME)
@printf "$(SILVER)[$(PURPLE)Binary file$(BLUE) $(notdir $(NAME))$(SILVER)] $(GREEN)Created.$(DEF)\n"
endef

define remove
@rm -rf $2
@printf "$(SILVER)[$(PURPLE)$1$(YELLOW)$2$(SILVER)] $(RED)deleted.$(DEF)\n"
endef

all: $(NAME)

$(NAME): $(OBJS)
	@make --no-print-directory -C $(PATH_LIB)
	@$(call compile_binary)

$(PATH_OBJ)%.o: $(PATH_SRC)%.c $(INC)
	@$(call compile_obj, $<, $@)

clean:
	@make clean --no-print-directory -C $(PATH_LIB)
	@$(call remove,Path objects, $(PATH_OBJ))

fclean: clean
	@make fclean --no-print-directory -C $(PATH_LIB)
	@$(call remove,Binary file, $(NAME))

re: fclean all

.PHONY: re fclean clean all
