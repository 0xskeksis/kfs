CC			= 	clang
		
CFLAGS		=	--target=i386-elf

SRCS_DIRS 	:= 		ctype \
					stdio \
				 	stdlib \
				 	string \

SRCS_DIRS	:= $(addprefix src/, $(SRCS_DIRS))

SRCS		:= $(foreach DIR, $(SRCS_DIRS), $(DIR)/$(wildcard $(DIR)/*.c))

BUILD_DIR	:= .build

OBJS  		= $(addprefix $(BUILD_DIR)/, $(SRCS:%.c=%.o))

DIR_UP		:=	mkdir -p $(@)

LIBNAME	:= klibc.a

$(info $(OBJS)) 

all:



