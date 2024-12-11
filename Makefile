NAME := decoder
.DEFAULT_GOAL := all
CPP := gcc
AR := ar
RM := rm -rf

################################################################################
###############                  DIRECTORIES                      ##############
################################################################################

OBJ_DIR := _obj
INC_DIRS := Includes
SRC_DIRS := src

# Tell the Makefile where headers and source files are
vpath %.h $(INC_DIRS)
vpath %.c $(SRC_DIRS)

################################################################################
###############                  SOURCE FILES                     ##############
################################################################################

SRC :=	main.c decoder.c
SRCS := $(addprefix src/, $(SRC))

# SRCS += $(BONUS_SRCS) $(GC_SRCS) $(GNL_SRCS)
# SRCS += $(EXTRA_SRC)

OBJS := $(addprefix $(OBJ_DIR)/, $(SRCS:%.c=%.o))
# GNL_OBJS := $(addprefix $(OBJ_DIR)/, $(GNL_SRCS:%.c=%.o))
# BONUS_OBJS := $(addprefix $(OBJ_DIR)/, $(BONUS_SRCS:%.c=%.o))
# GC_OBJS := $(addprefix $(OBJ_DIR)/, $(GC_SRCS:%.c=%.o))

################################################################################
########                         COMPILING                      ################
################################################################################

# CFLAGS := -Wall -Wextra -Werror -g -MMD -MP -I$(INC_DIRS)
CFLAGS := -Wall -Werror -Wextra -g  $(addprefix -I, $(INC_DIRS))
CFLAGS_SAN := $(CFLAGS) -fsanitize=address
LDFLAGS := -lncurses
LDFLAGS_SAN := -lncurses -fsanitize=address
ARFLAGS := -rcs

# ANSI color codes
GREEN := \033[0;32m
BOLD := \033[1m
NC := \033[0m # No Color

all: $(NAME)

$(NAME): $(OBJS)
#$(AR) $(ARFLAGS) $(NAME) $(OBJS)
	$(CPP) $(LDFLAGS) $(OBJS) -o $(NAME)
	@echo "$(BOLD)$(GREEN)Successful Compilation$(NC)"

# Rule to compile .o files
$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	mkdir -p $(@D)
	$(CPP) $(CFLAGS) -c $< -o $@

# Ensure the directories exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all
# submodule_update all

submodule_update:
	git submodule update --remote --merge

bonus: all

san: fclean
	make CFLAGS="$(CFLAGS_SAN)" LDFLAGS="$(LDFLAGS_SAN)"
	@echo "$(BOLD)$(GREEN)Successful Compilation with fsan$(NC)"

re_sub: submodule_rebuild

submodule_rebuild:
	git submodule deinit -f .
	git submodule update --init --recursive

-include $(OBJS:%.o=%.d)

.PHONY: all clean fclean re bonus re_sub submodule_rebuild san
