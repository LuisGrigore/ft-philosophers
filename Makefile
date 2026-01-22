CC      = cc
CFLAGS  = -Wall -Wextra -Werror
INCLUDES=  -I./bool -I./includes

OBJ_DIR = obj
NAME    = philo

SRCS= ./src/simulate.c ./src/init_table.c ./src/exit.c ./src/input_parser.c ./src/safe_thred.c ./src/safe_malloc.c ./src/main.c ./src/safe_mutex.c

OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re subsystems
