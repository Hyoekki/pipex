CC = cc
CFLAGS = -Wall -Werror -Wextra -O2
NAME = pipex
SRC = src/pipex.c \
	src/utils.c
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
OBJ = $(SRC:.c=.o)
INC = -Iinclude -I$(LIBFT_DIR)/include

all: $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(NAME): $(LIBFT) $(OBJ) 
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME)

clean:
	rm -rf $(OBJ)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
