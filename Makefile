PIPEX_SOURCES	=	pipex.c split_args.c pipex_utils.c \
					ft_split.c paths.c printf_min.c commands.c strings.c \
					forks.c
GNL_SOURCES		=	get_next_line.c get_next_line_utils.c
LST_SOURCES		=	ft_lstadd_front.c ft_lstclear.c ft_lstdelone.c ft_lstnew.c
GNL_DIR			=	./gnl/
LST_DIR			=	./lst/
SOURCES			=	$(PIPEX_SOURCES) $(addprefix $(GNL_DIR),$(GNL_SOURCES)) \
					$(addprefix $(LST_DIR),$(LST_SOURCES))
PIPEX_HEADERS	=	pipex.h
GNL_HEADERS		=	get_next_line.h
LST_HEADERS		=	ft_lst.h
HEADERS			=	$(PIPEX_HEADERS) $(addprefix $(GNL_DIR),$(GNL_HEADERS)) \
					$(addprefix $(LST_DIR),$(LST_HEADERS))
NAME			=	pipex
OBJECTS			=	$(SOURCES:.c=.o)
CFLAGS			=	-g -Wall -Wextra -Werror -I$(GNL_DIR) -I$(LST_DIR)
.PHONY:		all clean fclean re norm bonus

$(NAME):	$(OBJECTS) $(HEADERS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME)

all:		$(NAME)

bonus:		$(NAME)

%.o:		%.c $(HEADERS)
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJECTS)

fclean:		clean
	rm -f $(NAME)

re:			fclean all

norm:
	norminette $(SOURCES) $(HEADERS)