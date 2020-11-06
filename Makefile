# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/10/18 12:00:15 by alzaynou          #+#    #+#              #
#    Updated: 2020/11/06 14:14:53 by alzaynou         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_ls
GCC = gcc -Wall -Werror -Wextra
LIB = -lftdprintf
P_LIB = ./ft_dprintf/
INC_LIB = ./ft_dprintf/includes/
INC = ./includes/
P_SRC = ./src/

SRC = main.c \
	  error.c\
	  push.c\
	  print.c\
      push_dir.c\
	  open_dir.c
OBJ =  $(addprefix $(P_SRC), $(SRC))
all:
	@make --no-print-directory -C $(P_LIB)
	@$(GCC) $(OBJ) -I $(INC) -I $(INC_LIB) -L $(P_LIB) $(LIB) -o $(NAME)

re:
	@make re --no-print-directory -C $(P_LIB)
	@$(GCC) $(OBJ) -I $(INC) -I $(INC_LIB) -L $(P_LIB) $(LIB) -o $(NAME)

fclean:
		@make fclean --no-print-directory -C $(P_LIB)
		@rm -rf ft_ls

clean:
		@make clean --no-print-directory -C $(P_LIB)
