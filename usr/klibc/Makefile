TOOLCHAIN   := i386-elf
CC          := $(TOOLCHAIN)-gcc
		
CFLAGS		=	-ffreestanding      \
               -fno-builtin        \
               -fno-stack-protector \

IFLAGS		=	-I ./include -I ../../include/

SRCS_DIRS 	:= 		ctype \
					stdio \
				 	stdlib \
				 	string \

SRCS_DIRS	:= $(addprefix src/, $(SRCS_DIRS))

SRCS		:= $(foreach DIR, $(SRCS_DIRS), $(wildcard $(DIR)/*.c))

BUILD_DIR	:= .build

OBJS  		= $(addprefix $(BUILD_DIR)/, $(SRCS:%.c=%.o))

DIR_UP		=	mkdir -p $(@D)

NAME	:= klibc.a

RM		:= rm -rf

all: $(NAME)

$(NAME): $(OBJS)
	@ar rcs $(NAME) $^
$(BUILD_DIR)/%.o: %.c
	@$(DIR_UP)
	@$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@
clean:
	@$(RM) $(BUILD_DIR)
fclean: clean
	@$(RM) $(NAME)
re: fclean all
