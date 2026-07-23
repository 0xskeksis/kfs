NAME        := kfs
ISO         := $(NAME).iso

TOOLCHAIN   := i386-elf
CC          := $(TOOLCHAIN)-gcc
AS          := $(TOOLCHAIN)-as

BUILD_DIR   := .build
SRC_DIRS   := 	src \
				src/io \
				src/utils \

# SRC_DIRS	:= $(addprefix src/, $(SRC_DIRS))
INC_DIR     := include \
			   usr/klibc/include \

ISO_DIR     := isodir

LINKER      := linker.ld

GRUB_CFG    := grub.cfg

C_SRCS		:= $(foreach DIR, $(SRC_DIRS), $(wildcard $(DIR)/*.c))

# $(info $(C_SRCS))

# Pour l'instant je le fais a la main mais si yen a trop je changerais
ASM_SRCS    := src/bootloader.s \

C_OBJS  	= $(addprefix $(BUILD_DIR)/, $(C_SRCS:%.c=%.o))
ASM_OBJS  	= $(addprefix $(BUILD_DIR)/, $(ASM_SRCS:%.s=%.o))
OBJS        := $(C_OBJS) $(ASM_OBJS)

DEPS        := $(C_OBJS:.o=.d)

# IFLAGS    := -I$(INC_DIR)

IFLAGS    := $(foreach dir, $(INC_DIR), $(addprefix -I, $(dir)))

$(info $(IFLAGS))

CFLAGS      := -std=gnu11          \
               -ffreestanding      \
               -fno-builtin        \
               -fno-stack-protector \
               -Os                 \
               -MMD                \
               -MP					\

ASFLAGS     :=
LDFLAGS     := -T $(LINKER)        \
               -nostdlib           \
               -nodefaultlibs

ifeq ($(DEBUG),1)
	CFLAGS += -g
endif

ifeq ($(SILENCE),0)
	CFLAGS += -Wall -Wextra -Werror
endif

RM          := rm -rf
MKDIR       := mkdir -p
CP          := cp

Q           := @

KERNEL_LIB_PATH	:=	usr/klibc
KERNEL_LIB_NAME	:=	$(KERNEL_LIB_PATH)/klibc.a

LDLIBS      := $(KERNEL_LIB_NAME)

MAKEFLAGS = --no-print-directory

ifeq ($(VERBOSE),1)
	Q :=
endif

.PHONY: all iso run clean fclean re check

all: $(NAME)

$(NAME): $(OBJS) $(LINKER) $(KERNEL_LIB_NAME)
	@printf " $(GREEN)$(BOLD)■$(RESET)  linking    $(NAME)\n"
	$(Q)$(CC) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $@

$(BUILD_DIR)/%.o: %.c
	@printf " $(CYAN)$(BOLD)■$(RESET)  compiling  $<\n"
	$(Q)$(MKDIR) $(@D)
	$(Q)$(CC) $(IFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.s
	@printf " $(CYAN)$(BOLD)■$(RESET)  assembling $<\n"
	$(Q)$(MKDIR) $(@D)
	$(Q)$(AS) $(ASFLAGS) $< -o $@

$(KERNEL_LIB_NAME): $(KERNEL_LIB_PATH)
	$(Q)@printf " $(CYAN)$(BOLD)■$(RESET)  compiling  $<\n"
	$(Q)make -C $(KERNEL_LIB_PATH)

iso: $(ISO)

$(ISO): $(NAME) $(GRUB_CFG)
	@printf " $(GREEN)$(BOLD)■$(RESET)  creating   $@\n"
	$(Q)$(MKDIR) $(ISO_DIR)/boot/grub
	$(Q)$(CP) $(NAME) $(ISO_DIR)/boot/$(NAME)
	$(Q)$(CP) $(GRUB_CFG) $(ISO_DIR)/boot/grub/grub.cfg
	$(Q)grub-mkrescue -o $@ $(ISO_DIR)

check: $(NAME)
	@grub-file --is-x86-multiboot $(NAME)
	@printf " $(GREEN)$(BOLD)■$(RESET)  multiboot header detected\n"

run: $(ISO)
	qemu-system-i386 -cdrom $(ISO)

clean:
	@printf " $(RED)$(BOLD)■$(RESET)  removing   $(BUILD_DIR) $(ISO_DIR)\n"
	$(Q)$(RM) $(BUILD_DIR) $(ISO_DIR)
	$(Q)make clean -C $(KERNEL_LIB_PATH)

fclean: clean
	@printf " $(RED)$(BOLD)■$(RESET)  removing   $(NAME) $(ISO)\n"
	$(Q)$(RM) $(NAME) $(ISO)
	$(Q)make fclean -C $(KERNEL_LIB_PATH)

re: fclean all

-include $(DEPS)

BLACK       := \033[30m
RED         := \033[31m
GREEN       := \033[32m
YELLOW      := \033[33m
BLUE        := \033[34m
MAGENTA     := \033[35m
CYAN        := \033[36m
WHITE       := \033[37m
GRAY        := \033[90m

BOLD        := \033[1m
ITALIC      := \033[3m
RESET       := \033[0m
