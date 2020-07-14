NAME = ft_ls
GCC = gcc -g
INC =  include/
INC_L = ft_dprintf/includes
P_LIB = ft_dprintf/
P_SRC = src/
SRC = main.c \
      open_dir.c\
      print.c

OBJ = $(addprefix $(P_SRC), $(SRC))

all:
	$(GCC) $(OBJ) -L $(P_LIB) -lftdprintf -I $(INC) -I $(INC_L) -o $(NAME)
