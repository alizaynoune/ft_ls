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

NAME = ft_ls
GCC = gcc -g -Wall -Werror -Wextra
LIB = -lftdprintf
P_LIB = ./ft_dprintf/
INC_LIB = ./ft_dprintf/includes/
INC = ./includes/
P_SRC = ./src/

SRC =	free_error.c\
		get_lines.c\
		init.c\
		links.c\
		main.c\
		open_dir.c\
		parse_arg.c\
		parse_files.c\
		parse_options.c\
		print.c\
		print_helper.c\
		push.c\
		sort.c\
		tools.c

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
