CC	=	gcc

RM	=	rm -f

SRC	=	main.c \
		mods.c \
		menu.c \
		tir.c \
		parser.c \
		display.c \
		filesystem.c \
		liste_vaisseaux.c \
		liste_niveaux.c \
		pt.c

NAME	=	termInvaders

OBJ	=	$(SRC:.c=.o)

CFLAGS	=	-Wall -g3

all:	$(NAME)

$(NAME):	$(OBJ)
	$(CC) -o $(NAME) $(OBJ)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re:	fclean all

.PHONY: all clean fclean re
