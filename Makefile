# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: besoares <besoares@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/11/23 16:37:56 by besoares          #+#    #+#              #
#    Updated: 2026/11/23 16:40:21 by besoares         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        := codexion
CC          := cc
CFLAGS      := -Wall -Wextra -Werror -pthread
INC         := -I include
SRCS        := main.c \
               parse.c \
			   actions.c \
	    	   time.c \
			   init.c \
	       	   simulation.c \
			   heap.c \
			   scheduler.c

OBJS        := $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
