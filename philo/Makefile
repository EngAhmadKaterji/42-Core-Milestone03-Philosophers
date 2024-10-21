NAME = philo

SRCS = philo.c philo_life_utils.c philo_life.c philo_monitor.c philo_utils.c
OBJS = $(SRCS:.c=.o)
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread

all: $(NAME)

$(NAME): $(OBJS)
	@echo "Compiling and Linking..."
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "Compiled and Linked successfully!"

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning object files..."
	@rm -f $(OBJS)

fclean: clean
	@echo "Removing $(NAME)..."
	@rm -f $(NAME)

re: fclean all
