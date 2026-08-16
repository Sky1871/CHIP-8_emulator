NAME		= CHIP-8_emulator

CC			= cc
GDB			= gdb
RM			= rm -rf
MKDIR		= mkdir -p

SRC_DIR		= srcs
INC_DIR		= includes
OBJ_DIR		= obj

SDL_CFLAGS	= $(shell pkg-config --cflags sdl2)
SDL_LIBS	= $(shell pkg-config --libs sdl2)

CFLAGS		= -Wall -Wextra -Werror $(SDL_CFLAGS)
CPPFLAGS	= -I$(INC_DIR) -MMD -MP
LDFLAGS		=
LDLIBS		= $(SDL_LIBS)

DEBUG		?= 0

ifeq ($(DEBUG),1)
CFLAGS		+= -g3 -O0
CPPFLAGS	+= -DDEBUG=1
endif

SAN_FLAGS	= -fsanitize=address,undefined -fno-omit-frame-pointer

SRCS		= $(shell find $(SRC_DIR) -type f -name '*.c' | sort)
OBJS		= $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
DEPS		= $(OBJS:.o=.d)

COMPILE_DB	= compile_commands.json

JOBS		?= $(shell nproc)
MAKEFLAGS	+= -j $(JOBS) -l $(JOBS)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(MKDIR) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

run: $(NAME)
	./$(NAME)

debug:
	$(MAKE) fclean
	$(MAKE) DEBUG=1
	$(GDB) ./$(NAME)

san:
	$(MAKE) fclean
	$(MAKE) \
		CFLAGS="$(CFLAGS) -g3 -O1 $(SAN_FLAGS)" \
		LDFLAGS="$(LDFLAGS) $(SAN_FLAGS)"

compiledb:
	$(MAKE) fclean
	compiledb -n make

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)
	$(RM) $(COMPILE_DB)

re:
	$(MAKE) fclean
	$(MAKE) all

-include $(DEPS)

.PHONY: all run debug san compiledb clean fclean re
