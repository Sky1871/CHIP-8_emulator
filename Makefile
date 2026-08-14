NAME		= CHIP-8_emulator
EXE		= ./${NAME}
SRCS		= ${wildcard srcs/*.c}
OBJS		= ${SRCS:.c=.o}
CC		= gcc
GDB		= gdb
CFLAGS		= -Wall -Wextra -Werror ${shell pkg-config --cflags sdl2}
LDFLAGS		= ${shell pkg-config --libs sdl2}
CPPFLAGS	= -MMD -MP
RM		= rm -f

all: ${NAME}

run: re
	${EXE}

debug: CFLAGS += -g
debug: re
	${GDB} ${EXE}

%.o: %.c
	${CC} ${CFLAGS} ${CPPFLAGS} -c $< -o $@

${NAME}: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o ${NAME} ${LDFLAGS}

clean:
	${RM} ${OBJS} ${OBJS:.o=.d}

fclean : clean
	${RM} ${NAME}

re: fclean all

-include ${OBJS:.o=.d}

.PHONY: all clean fclean re debug run
