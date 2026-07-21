NAME        := kfs
ISO         := $(NAME).iso

TOOLCHAIN   := i386-elf-
CC          := $(TOOLCHAIN)gcc
AS          := $(TOOLCHAIN)as

BUILD_DIR   := .build
SRC_DIR     := src
INC_DIR     := include
ISO_DIR     := isodir

LINKER      := linker.ld
GRUB_CFG    := grub.cfg

# Ici peut-etre mettre un .mk dans chaque dossier et les importer histoire de pas avoir 1000 fichiers
#

include src/kernel/files.mk

C_SRCS      := $(SRC_DIR)/main.c \

ASM_SRCS    := $(SRC_DIR)/bootloader.s

C_OBJS      := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_SRCS))
ASM_OBJS    := $(patsubst $(SRC_DIR)/%.s,$(BUILD_DIR)/%.o,$(ASM_SRCS))
OBJS        := $(C_OBJS) $(ASM_OBJS)

DEPS        := $(C_OBJS:.o=.d)

IFLAGS    := -I$(INC_DIR)
CFLAGS      := -std=gnu11          \
               -ffreestanding      \
               -fno-builtin        \
               -fno-stack-protector \
               -O0                 \
               -MMD                \
               -MP

ASFLAGS     :=
LDFLAGS     := -T $(LINKER)        \
               -nostdlib           \
               -nodefaultlibs
LDLIBS      := -lgcc

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

ifeq ($(VERBOSE),1)
	Q :=
endif

.PHONY: all iso run clean fclean re check

all: $(NAME)

$(NAME): $(OBJS) $(LINKER)
	@printf " $(GREEN)$(BOLD)■$(RESET)  linking    $(NAME)\n"
	$(Q)$(CC) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@printf " $(CYAN)$(BOLD)■$(RESET)  compiling  $<\n"
	$(Q)$(MKDIR) $(@D)
	$(Q)$(CC) $(IFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s
	@printf " $(CYAN)$(BOLD)■$(RESET)  assembling $<\n"
	$(Q)$(MKDIR) $(@D)
	$(Q)$(AS) $(ASFLAGS) $< -o $@

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

fclean: clean
	@printf " $(RED)$(BOLD)■$(RESET)  removing   $(NAME) $(ISO)\n"
	$(Q)$(RM) $(NAME) $(ISO)

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
